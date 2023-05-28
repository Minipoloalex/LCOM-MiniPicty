#pragma once

#include "font/uppercase_a.xpm"
#include "font/uppercase_b.xpm"
#include "font/uppercase_c.xpm"
#include "font/uppercase_d.xpm"
#include "font/uppercase_e.xpm"
#include "font/uppercase_f.xpm"
#include "font/uppercase_g.xpm"
#include "font/uppercase_h.xpm"
#include "font/uppercase_i.xpm"
#include "font/uppercase_j.xpm"
#include "font/uppercase_k.xpm"
#include "font/uppercase_l.xpm"
#include "font/uppercase_m.xpm"
#include "font/uppercase_n.xpm"
#include "font/uppercase_o.xpm"
#include "font/uppercase_p.xpm"
#include "font/uppercase_q.xpm"
#include "font/uppercase_r.xpm"
#include "font/uppercase_s.xpm"
#include "font/uppercase_t.xpm"
#include "font/uppercase_u.xpm"
#include "font/uppercase_v.xpm"
#include "font/uppercase_w.xpm"
#include "font/uppercase_x.xpm"
#include "font/uppercase_y.xpm"
#include "font/uppercase_z.xpm"

#include "font/number_0.xpm"
#include "font/number_1.xpm"
#include "font/number_2.xpm"
#include "font/number_3.xpm"
#include "font/number_4.xpm"
#include "font/number_5.xpm"
#include "font/number_6.xpm"
#include "font/number_7.xpm"
#include "font/number_8.xpm"
#include "font/number_9.xpm"

#define FONT_HEIGHT 35 /**< @brief Height of each letter */
#define FONT_WIDTH 26 /**< @brief Width of each letter */
#define ALPHABET_SIZE 36 /**< @brief Number of letters in the alphabet */

/**
 * @brief Alphabet and numbers xpm array
 * Contains ALPHABET_SIZE xpms
 */
static xpm_map_t uppercase_alphabet[ALPHABET_SIZE] = {
  uppercase_a_xpm,
  uppercase_b_xpm,
  uppercase_c_xpm,
  uppercase_d_xpm,
  uppercase_e_xpm,
  uppercase_f_xpm,
  uppercase_g_xpm,
  uppercase_h_xpm,
  uppercase_i_xpm,
  uppercase_j_xpm,
  uppercase_k_xpm,
  uppercase_l_xpm,
  uppercase_m_xpm,
  uppercase_n_xpm,
  uppercase_o_xpm,
  uppercase_p_xpm,
  uppercase_q_xpm,
  uppercase_r_xpm,
  uppercase_s_xpm,
  uppercase_t_xpm,
  uppercase_u_xpm,
  uppercase_v_xpm,
  uppercase_w_xpm,
  uppercase_x_xpm,
  uppercase_y_xpm,
  uppercase_z_xpm,
  number_0_xpm,
  number_1_xpm,
  number_2_xpm,
  number_3_xpm,
  number_4_xpm,
  number_5_xpm,
  number_6_xpm,
  number_7_xpm,
  number_8_xpm,
  number_9_xpm
};
