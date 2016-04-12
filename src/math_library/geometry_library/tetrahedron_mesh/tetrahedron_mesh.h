//-*-C++-*-

#ifndef MATH_LIBRARY_GEOMETRY_LIBRARY_TETRAHEDRON_MESH_TETRAHEDRON_MESH_H
#define MATH_LIBRARY_GEOMETRY_LIBRARY_TETRAHEDRON_MESH_TETRAHEDRON_MESH_H
#include "math_library/geometry_library/tetrahedron_mesh/tetrahedron.hpp"
#include "math_library/geometry_library/tetrahedron_mesh/simplex.h"
#include "math_library/geometry_library/tetrahedron_mesh/facet.h"
#include "math_library/geometry_library/tetrahedron_mesh/tetrahedron_mesh_implementation/tetrahedron_mesh_initializer_2D.h"
//#include
//"math_library/geometry_library/tetrahedron_mesh/tetrahedron_mesh_implementation/tetrahedron_mesh_initializer_3D.h"

namespace math_algorithms {
/*!
 *  \defgroup TETRAHEDRON-MESH
 *  \ingroup  ALGORITHMS
 */

/*! \class   tetrahedron_mesh
 *  \ingroup TETRAHEDRON-MESH
 *
 *  \author  Peter Staar
 *  \brief   This class constructs a tetrahedron mesh for the Brillouin-zone, defined by the
 * k-cluster template.
 */
template <typename cluster_type>
class tetrahedron_mesh {
public:
  const static int DIMENSION = cluster_type::DIMENSION;

  typedef tetrahedron<DIMENSION> tetrahedron_t;
  typedef simplex<DIMENSION> simplex_t;
  typedef facet<DIMENSION> facet_t;
  typedef std::vector<double> vector_t;

public:
  tetrahedron_mesh(int n_recursion);
  ~tetrahedron_mesh();

  int size();
  tetrahedron<DIMENSION>& operator[](int l);

  bool is_inside_volume(std::vector<double> k_vec);

  std::vector<simplex_t>& get_simplices() {
    return simplices;
  };
  std::vector<facet_t>& get_facets() {
    return facets;
  };

  std::vector<std::vector<double>>& get_mesh() {
    return mesh;
  };

  std::vector<tetrahedron<DIMENSION>>& get_tetrahedra() {
    return tetrahedra;
  };

  void translate_simplices(std::vector<double> K);

  static void translate_mesh(std::vector<std::vector<double>>& centered_mesh,
                             std::vector<std::vector<double>>& translated_mesh,
                             std::vector<double>& K);

  bool check_consistency();

  void plot_simplices();
  void plot_mesh_points();
  void plot_facets();
  void plot_tetrahedra();

private:
  std::vector<simplex_t> simplices;
  std::vector<facet_t> facets;

  std::vector<vector_t> mesh;
  std::vector<tetrahedron_t> tetrahedra;

  int N_recursion;
};

template <typename cluster_type>
tetrahedron_mesh<cluster_type>::tetrahedron_mesh(int n_recursion)
    : simplices(0),
      facets(0),
      mesh(0),
      tetrahedra(0),

      N_recursion(n_recursion) {
  tetrahedron_mesh_initializer<DIMENSION, cluster_type> initializer(simplices, facets, mesh,
                                                                    tetrahedra, N_recursion);

  initializer.execute();

  assert(check_consistency());
}

template <typename cluster_type>
tetrahedron_mesh<cluster_type>::~tetrahedron_mesh() {}

template <typename cluster_type>
int tetrahedron_mesh<cluster_type>::size() {
  return tetrahedra.size();
}

template <typename cluster_type>
typename tetrahedron_mesh<cluster_type>::tetrahedron_t& tetrahedron_mesh<cluster_type>::operator[](
    int l) {
  assert(l > -1 and l < tetrahedra.size());
  return tetrahedra[l];
}

template <typename cluster_type>
void tetrahedron_mesh<cluster_type>::translate_simplices(std::vector<double> K) {
  for (size_t i = 0; i < simplices.size(); i++)
    for (int j = 0; j < DIMENSION; j++)
      simplices[i].k_vec[j] += K[j];
}

template <typename cluster_type>
void tetrahedron_mesh<cluster_type>::translate_mesh(std::vector<std::vector<double>>& centered_mesh,
                                                    std::vector<std::vector<double>>& translated_mesh,
                                                    std::vector<double>& K) {
  for (size_t i = 0; i < centered_mesh.size(); i++)
    for (size_t j = 0; j < centered_mesh[i].size(); j++)
      translated_mesh[i][j] = centered_mesh[i][j] + K[j];
}

template <typename cluster_type>
bool tetrahedron_mesh<cluster_type>::is_inside_volume(std::vector<double> k_vec) {
  assert(int(k_vec.size()) == DIMENSION);

  bool is_inside = true;

  int coor[DIMENSION];
  double pars[DIMENSION + 1];

  for (size_t l = 0; l < facets.size(); l++) {
    for (int d = 0; d < DIMENSION; d++)
      coor[d] = facets[l].index[d];

    facet_t::find_linear_parameters(coor, pars, simplices);

    double result = pars[DIMENSION];

    for (int d = 0; d < DIMENSION; d++)
      result += pars[d] * k_vec[d];

    if (result < -1.e-6)
      is_inside = false;
  }

  return is_inside;
}

template <typename cluster_type>
bool tetrahedron_mesh<cluster_type>::check_consistency() {
  bool OK = true;

  for (size_t l = 0; l < tetrahedra.size(); l++) {
    OK = tetrahedra[l].check_consistency(mesh);

    if (!OK)
      throw std::logic_error(__FUNCTION__);
  }

  return OK;
}

template <typename cluster_type>
void tetrahedron_mesh<cluster_type>::plot_simplices() {
  std::cout << std::scientific;
  std::cout.precision(6);

  for (size_t l = 0; l < simplices.size(); l++) {
    std::cout << l << "\t";

    for (size_t i = 0; i < simplices[l].k_vec.size(); i++)
      std::cout << simplices[l].k_vec[i] << "\t";

    std::cout << "\n";
  }

  std::cout << "\n";
}

template <typename cluster_type>
void tetrahedron_mesh<cluster_type>::plot_facets() {
  std::cout << std::scientific;
  std::cout.precision(6);

  for (size_t l = 0; l < facets.size(); l++) {
    std::cout << l << "\t";

    for (size_t i = 0; i < facets[l].index.size(); i++)
      std::cout << facets[l].index[i] << "\t";
    std::cout << "\n";
  }

  std::cout << "\n";
}

template <typename cluster_type>
void tetrahedron_mesh<cluster_type>::plot_mesh_points() {
  std::cout.precision(6);

  for (size_t l = 0; l < mesh.size(); l++) {
    std::cout << l << "\t";

    for (size_t i = 0; i < mesh[l].size(); i++)
      std::cout << mesh[l][i] << "\t";

    std::cout << "\n";
  }

  std::cout << "\n";
}

template <typename cluster_type>
void tetrahedron_mesh<cluster_type>::plot_tetrahedra() {
  std::cout.precision(6);

  for (size_t l = 0; l < tetrahedra.size(); l++) {
    for (size_t i = 0; i < DIMENSION + 1; i++)
      std::cout << tetrahedra[l].index[i] << "\t";
    std::cout << "\n";
  }

  std::cout << "\n";
}
}

#endif  // MATH_LIBRARY_GEOMETRY_LIBRARY_TETRAHEDRON_MESH_TETRAHEDRON_MESH_H
