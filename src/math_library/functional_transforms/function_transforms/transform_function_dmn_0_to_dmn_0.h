//-*-C++-*-

#ifndef MATH_LIBRARY_FUNCTIONAL_TRANSFORMS_FUNCTION_TRANSFORMS_TRANSFORM_FUNCTION_DMN_0_TO_DMN_0_H
#define MATH_LIBRARY_FUNCTIONAL_TRANSFORMS_FUNCTION_TRANSFORMS_TRANSFORM_FUNCTION_DMN_0_TO_DMN_0_H

#include <iostream>
#include "math_library/functional_transforms/domain_transforms/domain_transforms.hpp"
//#include "comp_library/linalg/linalg.hpp"
#include "comp_library/function_library/include_function_library.h"
//#include "math_library/typedefs.hpp"
using dca::util::SWAP_ALL;
using dca::util::SWAP_FIRST;
using dca::util::print_type;

namespace math_algorithms {
namespace functional_transforms {

template <typename type_input, typename type_output>
class TRANSFORM<dmn_0<type_input>, dmn_0<type_output>> {
  const static bool VERBOSE = false;

public:
  template <typename scalartype_input, class domain_input, typename scalartype_output, class domain_output>
  static void execute(FUNC_LIB::function<scalartype_input, domain_input>& f_input,
                      FUNC_LIB::function<scalartype_output, domain_output>& f_output) {
    if (VERBOSE)
      print_types(f_input, f_output);

    using TRANSFORMED_DOMAIN = typename SWAP_FIRST<domain_input, type_input, type_output>::Result;
    dca::util::assert_same<TRANSFORMED_DOMAIN, domain_output>();

    TRANSFORM_DOMAINWISE<domain_input, domain_output, type_input, type_output>::execute_on_first(
        f_input, f_output);
  }

  template <typename scalartype_input, class domain_input, typename scalartype_output,
            class domain_output, typename scalartype_T>
  static void execute(FUNC_LIB::function<scalartype_input, domain_input>& f_input,
                      FUNC_LIB::function<scalartype_output, domain_output>& f_output,
                      LIN_ALG::matrix<scalartype_T, LIN_ALG::CPU>& T) {
    if (VERBOSE)
      print_types(f_input, f_output);

    using TRANSFORMED_DOMAIN = typename SWAP_FIRST<domain_input, type_input, type_output>::Result;
    dca::util::assert_same<TRANSFORMED_DOMAIN, domain_output>();

    TRANSFORM_DOMAINWISE<domain_input, domain_output, type_input, type_output>::execute_on_first(
        f_input, f_output, T);
  }

  template <typename scalartype_input, class domain_input, typename scalartype_output, class domain_output>
  static void execute_on_all(FUNC_LIB::function<scalartype_input, domain_input>& f_input,
                             FUNC_LIB::function<scalartype_output, domain_output>& f_output) {
    if (VERBOSE)
      print_types(f_input, f_output);

    using TRANSFORMED_DOMAIN = typename SWAP_ALL<domain_input, type_input, type_output>::Result;

    dca::util::assert_same<TRANSFORMED_DOMAIN, domain_output>();

    TRANSFORM_DOMAINWISE<domain_input, domain_output, type_input, type_output>::execute_on_all(
        f_input, f_output);
  }

  template <typename scalartype_input, class domain_input, typename scalartype_output,
            class domain_output, typename scalartype_T>
  static void execute_on_all(FUNC_LIB::function<scalartype_input, domain_input>& f_input,
                             FUNC_LIB::function<scalartype_output, domain_output>& f_output,
                             LIN_ALG::matrix<scalartype_T, LIN_ALG::CPU>& T) {
    if (VERBOSE)
      print_types(f_input, f_output);

    using TRANSFORMED_DOMAIN = typename SWAP_ALL<domain_input, type_input, type_output>::Result;

    dca::util::assert_same<TRANSFORMED_DOMAIN, domain_output>();

    TRANSFORM_DOMAINWISE<domain_input, domain_output, type_input, type_output>::execute_on_all(
        f_input, f_output, T);
  }

private:
  template <typename scalartype_input, class domain_input, typename scalartype_output, class domain_output>
  static void print_types(FUNC_LIB::function<scalartype_input, domain_input>& f_input,
                          FUNC_LIB::function<scalartype_output, domain_output>& f_output,
                          bool do_all_domains = false) {
    typedef typename domain_input::this_type type_list_input;
    typedef typename domain_output::this_type type_list_output;

    print_type<type_input>::to_JSON(std::cout);
    std::cout << "\n\n";
    print_type<type_output>::to_JSON(std::cout);
    std::cout << "\n\n";

    if (do_all_domains) {
      print_type<type_list_input>::to_JSON(std::cout);
      std::cout << "\n\n";

      using TRANSFORMED_DOMAIN = typename SWAP_ALL<domain_input, type_input, type_output>::Result;

      print_type<typename TRANSFORMED_DOMAIN::this_type>::to_JSON(std::cout);
      std::cout << "\n\n";

      print_type<type_list_output>::to_JSON(std::cout);
      std::cout << "\n\n";

      FUNC_LIB::function<scalartype_output, TRANSFORMED_DOMAIN> T;
      T.print_fingerprint();
      f_output.print_fingerprint();
    }
    else {
      print_type<type_list_input>::to_JSON(std::cout);
      std::cout << "\n\n";

      using TRANSFORMED_DOMAIN = typename SWAP_FIRST<domain_input, type_input, type_output>::Result;

      print_type<typename TRANSFORMED_DOMAIN::this_type>::to_JSON(std::cout);
      std::cout << "\n\n";

      print_type<type_list_output>::to_JSON(std::cout);
      std::cout << "\n\n";

      FUNC_LIB::function<scalartype_output, TRANSFORMED_DOMAIN> T(
          "FUNC_LIB::function<scalartype_output, TRANSFORMED_DOMAIN>");

      f_input.print_fingerprint();
      T.print_fingerprint();
      f_output.print_fingerprint();
    }
  }
};

}  // functional_transforms
}  // math_algorithms

#endif
