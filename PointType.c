#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct PointColor {
  unsigned int isColor : 1;
  unsigned char A : 7;
  unsigned char R : 8;
  unsigned char G : 8;
  unsigned char B : 8;
};

union PointNode {
  struct PointColor leaf;
  struct PointFull *parent;
};

struct PointFull {
  struct PointColor color;
  union PointNode children[8];
};

int main(int argc, char *argv[]) {
  printf("PointColor: %lu\n", sizeof(struct PointColor));
  printf("PointFull: %lu\n", sizeof(struct PointFull));
  printf("PointNode: %lu\n", sizeof(union PointNode));

  struct PointColor pc;
  pc.isColor = 1;

  struct PointFull pf;
  pf.color.isColor = 0;

  union PointNode *pn;
  pn->leaf = pc;
  pn->parent = malloc(sizeof(struct PointFull));

  printf("PointNode isColor: %d\n", pn->leaf.isColor);

  return 0;
}
