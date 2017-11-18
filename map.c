
#include <math.h>

#define L 500
#define HIT 0b11
#define MISS 0b10

uint16_t mat[L][L] = {{0}};

void update_map (int x, int y, int dir, int values, int *obstacles, int *angles) {
  int i, j;
  int r, c;
  int w = 4;
  int t = 13;
  float height_ob = 1;
  float * obstaclesF;

  obstaclesF = (float *)malloc(sizeof(float)*values);

  for(i=0;i<values;i++){
      obstaclesF[i]= ((float)obstacles[i])/10.0;
  }

  for (i = 0; i < values; i++) {

    float mx = (obstacles[i] == 0 ? t : obstaclesF[i]) * sin((angles[i] * M_PI) / 180.0);
    float my = (obstacles[i] == 0 ? t : obstaclesF[i]) * cos((angles[i] * M_PI) / 180.0);
    float heightx = (obstacles[i] == 0 ? 0 : height_ob) * sin((angles[i] * M_PI) / 180.0);
    float heighty = (obstacles[i] == 0 ? 0 : height_ob) * cos((angles[i] * M_PI) / 180.0);

    float nx = w/2 * sin(((angles[i]+90) * M_PI) / 180.0);
    float ny = w/2 * cos(((angles[i]+90) * M_PI) / 180.0);

    float p1x = x - nx;
    float p1y = y - ny;
    // printf("P1 (%f, %f)\n", p1x, p1y);

    float p2x = x + nx;
    float p2y = y + ny;
    // printf("P2 (%f, %f)\n", p2x, p2y);

    float p3x = p2x + mx;
    float p3y = p2y + my;
    // printf("P3 (%f, %f)\n", p3x, p3y);

    float p4x = p1x + mx;
    float p4y = p1y + my;
    // printf("P4 (%f, %f)\n", p4x, p4y);

    float ob_p1x = p4x;
    float ob_p1y = p4y;
    // printf("P1 (%f, %f)\n", p1x, p1y);

    float ob_p2x = p3x;
    float ob_p2y = p3y;
    // printf("P2 (%f, %f)\n", p2x, p2y);

    float ob_p3x = ob_p2x + heightx;
    float ob_p3y = ob_p2y + heighty;
    // printf("P3 (%f, %f)\n", p3x, p3y);

    float ob_p4x = ob_p1x + heightx;
    float ob_p4y = ob_p1y + heighty;

    printf("coordinates: (%f, %f), (%f, %f), (%f, %f), (%f, %f)\n", ob_p1x, ob_p1y, ob_p2x, ob_p2y, ob_p3x, ob_p3y, ob_p4x, ob_p4y);



    for (r = x+t; r > x-t; r--) {
      for (c = x-t; c < x+t+1; c++) {
        if ( (p2y-p1y)*c - (p2x-p1x)*r + p2x*p1y - p2y*p1x <= 0 &&
             (p3y-p2y)*c - (p3x-p2x)*r + p3x*p2y - p3y*p2x <= 0 &&
             (p4y-p3y)*c - (p4x-p3x)*r + p4x*p3y - p4y*p3x <= 0 &&
             (p1y-p4y)*c - (p1x-p4x)*r + p1x*p4y - p1y*p4x <= 0 ) {

          mat[r][c] = (mat[r][c] << 2) + MISS; // o miss
          printf("0 ");
        } else if (!(heightx==0 && heighty==0) &&(ob_p2y-ob_p1y)*c - (ob_p2x-ob_p1x)*r + ob_p2x*ob_p1y - ob_p2y*ob_p1x <= 0 &&
             (ob_p3y-ob_p2y)*c - (ob_p3x-ob_p2x)*r + ob_p3x*ob_p2y - ob_p3y*ob_p2x <= 0 &&
             (ob_p4y-ob_p3y)*c - (ob_p4x-ob_p3x)*r + ob_p4x*ob_p3y - ob_p4y*ob_p3x <= 0 &&
             (ob_p1y-ob_p4y)*c - (ob_p1x-ob_p4x)*r + ob_p1x*ob_p4y - ob_p1y*ob_p4x <= 0 ) {

          mat[r][c] = (mat[r][c] << 2) + HIT;
          printf("1 ");
       }  else {
          // fuori dal campo visivo, rimane unknown
          printf("x ");
        }
      }
      printf("\n");
    }
    printf("------------------------------------------------------\n");
  }

}

void map_print(int startX, int startY, int endX, int endY) {
  int r, c, i;

  FILE * fp = stdout;//fopen("test.txt", "w+");
  for (r = endX; r > startX; r--) {
    for (c = startX; c < endX; c++) {
      uint16_t tmp = mat[r][c];
      for (i = 0; i < 8; i++) {
        switch ((tmp >> (7-i)*2) & 0b11)  {
          case (0):
            fprintf(fp, "00 ");
            break;
          case (1):
            fprintf(fp, "01 ");
            break;
          case (2):
            fprintf(fp, "10 ");
            break;
          case (3):
            fprintf(fp, "11 ");
            break;
        }
      }
      fprintf(fp, "| ");
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
}