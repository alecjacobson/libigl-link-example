#include <igl/matlab_format.h>
//#include <igl/matlab/MatlabWorkspace.h>
#include <igl/readDMAT.h>
#include <igl/get_seconds.h>

#include <igl/triangle/triangulate.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <igl/copyleft/cgal/triangulate.h>

int main(int argc, char * argv[])
{
  Eigen::MatrixXd V;
  Eigen::MatrixXi E;
  igl::readDMAT("Pl.dmat",V);
  igl::readDMAT("El.dmat",E);
  const auto & tictoc = []()
  {
    static double t_start = igl::get_seconds();
    double diff = igl::get_seconds()-t_start;
    t_start += diff;
    return diff;
  };
  const std::string flags = "cQ";

  Eigen::MatrixXd TV;
  Eigen::MatrixXi TF;
  tictoc();
  for(int iter = 0;iter<10;iter++)
  {
    igl::copyleft::cgal::triangulate<CGAL::Epeck>(V,E,Eigen::MatrixXi(),flags,TV,TF);
  }
  printf("Epeck:    %g secs\n",tictoc());
  printf("  TV: %ld, TF: %ld\n",TV.rows(),TF.rows());

  tictoc();
  for(int iter = 0;iter<10;iter++)
  {
    igl::copyleft::cgal::triangulate<CGAL::Epick>(V,E,Eigen::MatrixXi(),flags,TV,TF);
  }
  printf("Epick:    %g secs\n",tictoc());
  printf("  TV: %ld, TF: %ld\n",TV.rows(),TF.rows());

  tictoc();
  for(int iter = 0;iter<10;iter++)
  {
    igl::triangle::triangulate(V,E,Eigen::MatrixXi(),flags,TV,TF);
  }
  printf("triangle: %g secs\n",tictoc());
  printf("  TV: %ld, TF: %ld\n",TV.rows(),TF.rows());

  //std::cout<<igl::matlab_format(TV,"TV")<<std::endl;
  //std::cout<<igl::matlab_format_index(TF,"TF")<<std::endl;
  //
}
