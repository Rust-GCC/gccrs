/* { dg-additional-options "-std=c23 -m4 -mfsca -ffast-math" }  */
/* { dg-do compile }  */

typedef float vec3[3];
typedef float vec4[4];
typedef vec4 mat4[4];

float glm_rotate_make_angle, glm_rotate_make___trans_tmp_1, glm_rotate_at_m_2_1,
    glm_rotate_at_m_1_1, glm_rotate_at_m_0_0, glm_rotate_at_m_0_3,
    glm_rotate_at_m_2_3, glm_rotate_at_pivot;
vec3 glm_rotate_make_vs;

vec4 *glm_rotate_at_m;

inline void glm_rotate_make (mat4 m)
{
  float vs_2;
  glm_rotate_make___trans_tmp_1 = __builtin_sinf (glm_rotate_make_angle);
  float *dest = glm_rotate_make_vs;
  dest[0] = glm_rotate_make___trans_tmp_1;
  m[0][2] -= vs_2 += glm_rotate_make_vs[0];
}

void glmc_rotate_at (void)
{
  __attribute((aligned(8))) vec3 pivotInv;
  float *v = &glm_rotate_at_pivot, *dest = glm_rotate_at_m[3];
  dest[3] += glm_rotate_at_m[1][3] * v[1];
  dest[3] += glm_rotate_at_m_2_3 * v[2];
  {
    __attribute((aligned(16))) mat4 rot;
    glm_rotate_make(rot);
    vec4 *dest = glm_rotate_at_m;
    float a03 = glm_rotate_at_m_0_3, b10 = rot[1][0], b11 = rot[1][1];
    dest[0][1] = glm_rotate_at_m_0_0 + glm_rotate_at_m_1_1 * rot[0][1] +
                 glm_rotate_at_m_2_1 * rot[0][2];
    dest[1][3] = a03 * b10 * b11;
  }
  {
    dest[3] += glm_rotate_at_m_1_1;
    dest[3] += pivotInv[2];
  }
}
