/* { dg-additional-options "-std=c23 -m4 -mfsca -ffast-math" }  */
/* { dg-do compile }  */

/* N.B. This case was not posted in the PR but it's related since
        it's triggered by fsca insn and v2sf usage.  */

typedef float vec4[4];
float glm_euler_by_order_angles_2, glm_euler_by_order_angles_1,
    glm_euler_by_order_angles_0, glm_euler_by_order_dest_2_2,
    glm_euler_by_order_dest_2_1, glm_euler_by_order_dest_2_0,
    glm_euler_by_order_dest_0_2, glm_euler_by_order_dest_0_1,
    glm_euler_by_order_sy, glm_euler_by_order_sz, glm_euler_by_order_cysz,
    glm_euler_by_order_cysx, glm_euler_by_order_cxcy, glm_euler_by_order_cx,
    glm_euler_by_order_cy, glm_euler_by_order_cz, glm_euler_by_order_cycz;
vec4 *glm_euler_by_order_dest;

typedef enum {
  GLM_EULER_XZY = 2 << 4,
  GLM_EULER_ZXY = 4,
  GLM_EULER_ZYX
} glm_euler_seq;
glm_euler_seq glm_euler_by_order_ord;

void glm_euler_by_order()
{
  float czsy, sysz;
  glm_euler_by_order_cx = __builtin_cosf (glm_euler_by_order_angles_0);
  glm_euler_by_order_sy = __builtin_cosf (glm_euler_by_order_angles_1);
  glm_euler_by_order_sz = __builtin_cosf (glm_euler_by_order_angles_2);
  glm_euler_by_order_cycz = glm_euler_by_order_cy * glm_euler_by_order_cz;
  glm_euler_by_order_cysz = glm_euler_by_order_cy * glm_euler_by_order_sz;
  glm_euler_by_order_cysx = glm_euler_by_order_cy * glm_euler_by_order_angles_0;
  glm_euler_by_order_cxcy = glm_euler_by_order_cx * glm_euler_by_order_cy;
  czsy = glm_euler_by_order_cz * glm_euler_by_order_sy;

  switch (glm_euler_by_order_ord)
  {
  case GLM_EULER_XZY:
    glm_euler_by_order_dest_0_1 =
        glm_euler_by_order_cx * glm_euler_by_order_cysz;
    break;
  case GLM_EULER_ZXY:
    glm_euler_by_order_dest[0][0] =
        glm_euler_by_order_cycz - glm_euler_by_order_angles_0 * sysz;
    glm_euler_by_order_dest_0_2 = glm_euler_by_order_cx * glm_euler_by_order_sy;
    glm_euler_by_order_dest_2_0 =
        czsy + glm_euler_by_order_cysx * glm_euler_by_order_sz;
    glm_euler_by_order_dest_2_1 =
        -glm_euler_by_order_cycz * glm_euler_by_order_angles_0 + sysz;
    glm_euler_by_order_dest_2_2 = glm_euler_by_order_cxcy;
  case GLM_EULER_ZYX:
    glm_euler_by_order_dest[0][0] = glm_euler_by_order_cycz;
  }
}