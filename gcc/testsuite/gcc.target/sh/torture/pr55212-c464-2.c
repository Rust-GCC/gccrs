/* { dg-additional-options "-std=c23 -m4 -mfsca -ffast-math" }  */
/* { dg-do compile }  */


typedef float vec4[4];
typedef vec4 mat4[4];
float glm_mul_rot_m1_2_2, glm_mul_rot_m1_1_2, glm_mul_rot_m1_0_2,
    glm_mul_rot_m1_3_3, glm_mul_rot_m1_3_2, glm_mul_rot_m1_3_1,
    glm_mul_rot_m1_3_0, glm_mul_rot_m1_2_1, glm_mul_rot_m1_2_0,
    glm_mul_rot_dest_3_3, glm_mul_rot_dest_3_2, glm_mul_rot_dest_3_1,
    glm_mul_rot_dest_3_0, glm_mul_rot_dest_2_3, glm_mul_rot_dest_1_2,
    glm_mul_rot_dest_1_1, glm_mul_rot_dest_1_0, glm_mul_rot_dest_0_2,
    glm_rotate_z_angle;

static void glm_mul_rot(mat4 m2) {
  float a20 = glm_mul_rot_m1_2_0, a21 = glm_mul_rot_m1_2_1,
        a30 = glm_mul_rot_m1_3_0, a31 = glm_mul_rot_m1_3_1,
        a32 = glm_mul_rot_m1_3_2, a33 = glm_mul_rot_m1_3_3, b11 = m2[1][1],
        b20 = m2[2][0], b21 = m2[2][1], b22 = m2[2][2];
  glm_mul_rot_dest_0_2 = glm_mul_rot_m1_0_2 * m2[0][0] +
                         glm_mul_rot_m1_1_2 * m2[0][1] +
                         glm_mul_rot_m1_2_2 * m2[0][2];
  glm_mul_rot_dest_1_0 = a20;
  glm_mul_rot_dest_1_1 = a21;
  glm_mul_rot_dest_1_2 = b11;
  glm_mul_rot_dest_2_3 = b20 * b21 * b22;
  glm_mul_rot_dest_3_0 = a30;
  glm_mul_rot_dest_3_1 = a31;
  glm_mul_rot_dest_3_2 = a32;
  glm_mul_rot_dest_3_3 = a33;
}
void glmc_rotate_z() {
  mat4 t;
  float s = __builtin_sinf(glm_rotate_z_angle);
  t[0][1] = s;
  glm_mul_rot(t);
}