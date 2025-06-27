const sdl = @cImport({
    @cInclude("SDL3/SDL.h");
    @cDefine("SDL_MAIN_HANDLED", {});
    @cInclude("SDL3/SDL_main.h");
});

const VW = 640;
const VH = 480;

pub inline fn errify(value: anytype) error{SdlError}!switch (@typeInfo(@TypeOf(value))) {
    .Bool => void,
    .Pointer, .Optional => @TypeOf(value.?),
    .Int, .Float => @TypeOf(value),
    else => @compileError("unerrifiable type: " ++ @typeName(@TypeOf(value))),
} {
    return switch (@typeInfo(@TypeOf(value))) {
        .Bool => if (!value) error.SdlError,
        .Pointer, .Optional => value orelse error.SdlError,
        .Int, .Float => if (value == 0) return error.SdlError else value,
        else => comptime unreachable,
    };
}

const qmapping = struct {
    dev: *sdl.SDL_GPUDevice,
    win: *sdl.SDL_Window,
    pipeline: *sdl.SDL_GPUGraphicsPipeline,
    depthTex: *sdl.SDL_GPUTexture,

    fn LoadShader(
        dev: *sdl.SDL_GPUDevice,
        path: []const u8,
        stage: sdl.SDL_GPUShaderStage,
        num_samplers: u32,
        num_uniform_buffers: u32,
        num_storage_buffers: u32,
        num_storage_textures: u32,
    ) error{ SdlError, ShaderFormatError }!*sdl.SDL_GPUShader {
        const backendFormats: sdl.SDL_GPUShaderFormat = sdl.SDL_GetGPUShaderFormats(dev);
        if (backendFormats & sdl.SDL_GPU_SHADERFORMAT_SPIRV == 0)
            return error.ShaderFormatError;

        var len: usize = 0;
        const code: [*c]u8 = @ptrCast(try errify(sdl.SDL_LoadFile(path.ptr, &len)));

        const shaderInfo: sdl.SDL_GPUShaderCreateInfo = .{
            .code = code,
            .code_size = len,
            .entrypoint = "main",
            .format = sdl.SDL_GPU_SHADERFORMAT_SPIRV,
            .stage = stage,
            .num_samplers = num_samplers,
            .num_uniform_buffers = num_uniform_buffers,
            .num_storage_buffers = num_storage_buffers,
            .num_storage_textures = num_storage_textures,
            .props = 0,
        };
        const shader: *sdl.SDL_GPUShader = try errify(sdl.SDL_CreateGPUShader(dev, &shaderInfo));
        sdl.SDL_free(code);
        return shader;
    }

    pub fn init() !qmapping {
        const dev: *sdl.SDL_GPUDevice = try errify(sdl.SDL_CreateGPUDevice(sdl.SDL_GPU_SHADERFORMAT_SPIRV, false, null));

        const vert: *sdl.SDL_GPUShader = try LoadShader(dev, "shaders/basic.vert.spv", sdl.SDL_GPU_SHADERSTAGE_VERTEX, 0, 1, 0, 0);
        const frag: *sdl.SDL_GPUShader = try LoadShader(dev, "shaders/basic.frag.spv", sdl.SDL_GPU_SHADERSTAGE_FRAGMENT, 1, 0, 0, 0);

        const win: *sdl.SDL_Window = sdl.SDL_CreateWindow(",,", VW, VH, 0);
        sdl.SDL_ClaimWindowForGPUDevice(dev, win);

        const pipelineInfo: sdl.SDL_GPUGraphicsPipelineCreateInfo = .{
            .vertex_input_state = .{
                .num_vertex_buffers = 1,
                .vertex_buffer_descriptions = &[_]sdl.SDL_GPUVertexBufferDescription{
                    .{
                        .slot = 0,
                        .input_rate = sdl.SDL_GPU_VERTEXINPUTRATE_VERTEX,
                        .instance_step_rate = 0,
                        .pitch = @sizeOf(sdl.SDL_FPoint) * 2,
                    },
                },
                .num_vertex_attributes = 2,
                .vertex_attributes = &[_]sdl.SDL_GPUVertexAttribute{
                    .{
                        .buffer_slot = 0,
                        .format = sdl.SDL_GPU_VERTEXELEMENTFORMAT_FLOAT32,
                        .location = 0,
                        .offset = 0,
                    },
                    .{
                        .buffer_slot = 0,
                        .format = sdl.SDL_GPU_VERTEXELEMENTFORMAT_FLOAT32,
                        .location = 1,
                        .offset = @sizeOf(sdl.SDL_FPoint),
                    },
                },
            },
            .primitive_type = sdl.SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
            .vertex_shader = vert,
            .fragment_shader = frag,
            .rasterizer_state = .{
                .fill_mode = sdl.SDL_GPU_FILLMODE_FILL,
            },
            .multisample_state = .{
                .sample_count = sdl.SDL_GPU_SAMPLECOUNT_1,
            },
            .depth_stencil_state = .{
                .enable_depth_test = true,
                .enable_depth_write = true,
                .compare_op = sdl.SDL_GPU_COMPAREOP_LESS_OR_EQUAL,
                .enable_stencil_test = false,
            },
            .target_info = .{
                .num_color_targets = 1,
                .color_target_descriptions = &[_]sdl.SDL_GPUColorTargetDescription{
                    .{
                        .format = sdl.SDL_GetGPUSwapchainTextureFormat(dev, win),
                    },
                },
                .depth_stencil_format = sdl.SDL_GPU_TEXTUREFORMAT_D16_UNORM,
                .has_depth_stencil_target = true,
            },
        };
        const pipeline: *sdl.SDL_GPUGraphicsPipeline = sdl.SDL_CreateGPUGraphicsPipeine(dev, &pipelineInfo);
        const depthTex: *sdl.SDL_GPUTexture = sdl.SDL_CreateGPUTexture(dev, .{
            .type = sdl.SDL_GPU_TEXTURETYPE_2D,
            .width = VW,
            .height = VH,
            .layer_count_or_depth = 1,
            .num_levels = 1,
            .sample_count = sdl.SDL_GPU_SAMPLECOUNT_1,
            .format = sdl.SDL_GPU_TEXTUREFORMAT_D16_UNORM,
            .usage = sdl.SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
        });

        sdl.SDL_ReleaseGPUShader(dev, vert);
        sdl.SDL_ReleaseGPUShader(dev, frag);

        const ret: qmapping = .{
            .dev = dev,
            .win = win,
            .pipeline = pipeline,
            .depthTex = depthTex,
        };
        return ret;
    }

    pub fn deinit(self: qmapping) void {
        sdl.SDL_DestroyGPUDevice(self.dev);
    }

    pub fn AppEvent(self: qmapping, e: sdl.SDL_Event) bool {
        _ = self;
        if (e.type == sdl.SDL_EVENT_QUIT) return false;
        return true;
    }

    pub fn AppUpdate(self: qmapping) !void {
        const cmdbuf: *sdl.SDL_GPUCommandBuffer = try errify(sdl.SDL_AcquireGPUCommandBuffer(self.dev));
        var swapchainTexture: *sdl.SDL_GPUTexture = null;
        try errify(sdl.SDL_WaitAndAcquireGPUSwapchainTexture(cmdbuf, self.win, &swapchainTexture, null, null));
    }
};

pub fn main() !void {
    try errify(sdl.SDL_Init(sdl.SDL_INIT_VIDEO));
    sdl.SDL_Log("Started");
    defer sdl.SDL_Log("Quitting...");
    defer sdl.SDL_Quit();

    const q: qmapping = try qmapping.init();
    defer q.deinit();

    const window = try errify(sdl.SDL_CreateWindow(
        "redbed",
        1280,
        720,
        sdl.SDL_WINDOW_RESIZABLE,
    ));
    defer sdl.SDL_DestroyWindow(window);

    main_loop: while (true) {
        const e: sdl.SDL_Event = undefined;

        while (sdl.SDL_PollEvent(&e)) {
            if (!qmapping.AppEvent(e))
                break :main_loop;
        }
    }
}
