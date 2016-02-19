// Copyright 2016 ETH Zurich.
//
// Author: Urs Haehner (haehneru@itp.phys.ethz.ch), ETH Zurich

#ifndef GENERAL_INTERACTION_HPP
#define GENERAL_INTERACTION_HPP

#include <array>
#include <vector>

// change the class name later!!
class on_site_u {
 public:
  template <class vertex_pair_type, class parameters_type, class rng_type,
            class H_interaction_type>
  static void set_vertex(vertex_pair_type& vertex, parameters_type& parameters,
                         rng_type& rng, H_interaction_type H_interaction) {
    using b = typename parameters_type::b;
    using s = typename parameters_type::s;
    using r_DCA = typename parameters_type::r_DCA;
    using nu = typename parameters_type::nu;

    using b_s_b_s_r_r_dmn_t = dmn_6<b, s, b, s, r_DCA, r_DCA>;
    b_s_b_s_r_r_dmn_t b_s_b_s_r_r_dmn;

    static std::vector<int> correlated_orbitals =
        on_site_u::make_correlated_orbitals(parameters, H_interaction);

    // Get a random pair of correlated orbitals
    const int pos = rng.get_random_number() * correlated_orbitals.size();
    const int lin_ind = correlated_orbitals[pos];

    std::array<int, 6>
        sub_ind;  // [0]=b1, [1]=s1, [2]=b2, [3]=s2, [4]=r1, [5]=r2
    b_s_b_s_r_r_dmn.linind_2_subind(lin_ind, sub_ind.data());

    // Set the vertex properties
    vertex.get_bands().first = parameters.get_interacting_bands()[sub_ind[0]];
    vertex.get_bands().second = parameters.get_interacting_bands()[sub_ind[2]];

    vertex.get_e_spins().first =
        electron_spin_domain::get_elements()[sub_ind[1]];
    vertex.get_e_spins().second =
        electron_spin_domain::get_elements()[sub_ind[3]];

    vertex.get_spin_orbitals().first = QMC::convert<int, nu>::spin_orbital(
        vertex.get_bands().first,
        vertex.get_e_spins().first);  // nu = dmn_2<b,s>
    vertex.get_spin_orbitals().second = QMC::convert<int, nu>::spin_orbital(
        vertex.get_bands().second, vertex.get_e_spins().second);

    vertex.get_r_sites().first = sub_ind[4];
    vertex.get_r_sites().second = sub_ind[5];
  }

 private:
  template <class parameters_type, class H_interaction_type>
  static std::vector<int> make_correlated_orbitals(
      parameters_type& parameters, H_interaction_type& H_interaction) {
    using b = typename parameters_type::b;
    using s = typename parameters_type::s;
    using r_DCA = typename parameters_type::r_DCA;

    using b_s_b_s_r_r_dmn_t = dmn_6<b, s, b, s, r_DCA, r_DCA>;
    b_s_b_s_r_r_dmn_t b_s_b_s_r_r_dmn;

    std::vector<int> correlated_orbitals;

    for (int r_j = 0; r_j < r_DCA::dmn_size(); ++r_j) {
      for (int r_i = 0; r_i < r_DCA::dmn_size(); ++r_i) {
        int delta_r =
            r_DCA::parameter_type::subtract(r_j, r_i);  // delta_r = r_i - r_j

        for (int s_j = 0; s_j < s::dmn_size(); ++s_j) {
          for (int b_j = 0; b_j < b::dmn_size(); ++b_j) {
            for (int s_i = 0; s_i < s::dmn_size(); ++s_i) {
              for (int b_i = 0; b_i < b::dmn_size(); ++b_i) {
                if (std::abs(H_interaction(b_i, s_i, b_j, s_j, delta_r)) >
                    1.e-3) {
                  int linear_index =
                      b_s_b_s_r_r_dmn(b_i, s_i, b_j, s_j, r_i, r_j);
                  correlated_orbitals.push_back(linear_index);
                }
              }
            }
          }
        }
      }
    }
    return correlated_orbitals;
  }
};

#endif  // GENERAL_INTERACTION_HPP
