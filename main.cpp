#include <igl/read_triangle_mesh.h>
#ifdef LIBIGL_CGAL
#  include <igl/copyleft/cgal/convex_hull.h>
#  include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#endif
#ifdef LIBIGL_EMBREE
#  include <igl/embree/reorient_facets_raycast.h>
#  include <embree3/rtcore.h>
#endif
#ifdef LIBIGL_MATLAB
#  include <igl/matlab/MatlabWorkspace.h>
#  include <mat.h>
#endif
#ifdef LIBIGL_MOSEK
#  include <igl/mosek/bbw.h>
#  include <mosek.h>
#endif
#ifdef LIBIGL_GLFW
#  include <igl/opengl/glfw/Viewer.h>
#  ifdef LIBIGL_IMGUI
#    include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#    include <igl/opengl/glfw/imgui/ImGuiHelpers.h>
#  endif
#endif
#ifdef LIBIGL_PNG
#  include <igl/png/writePNG.h>
#endif
#ifdef LIBIGL_TETGEN
#  include <igl/copyleft/tetgen/tetrahedralize.h>
#  include <tetgen.h>
#endif
#ifdef LIBIGL_TRIANGLE
#  include <igl/triangle/triangulate.h>
#endif
#ifdef LIBIGL_XML
#  include <igl/xml/writeDAE.h>
#endif

int main(int argc, char * argv[])
{
  Eigen::MatrixXd V;
  Eigen::MatrixXi F;
  igl::read_triangle_mesh(argv[1],V,F);
#ifdef LIBIGL_CGAL
  Eigen::MatrixXi H;
  igl::copyleft::cgal::convex_hull(V,H);
#endif
#ifdef LIBIGL_EMBREE
  Eigen::MatrixXi FF;
  Eigen::VectorXi I;
  igl::embree::reorient_facets_raycast(V,F,FF,I);
#endif
#ifdef LIBIGL_MATLAB
  igl::matlab::MatlabWorkspace mw;
  mw.save(V,"V");
  mw.save_index(F,"F");
  mw.write("test.mat");
#endif
#ifdef LIBIGL_MOSEK
  const Eigen::VectorXi b = (Eigen::VectorXi(2)<<0,V.rows()-1).finished();
  const Eigen::MatrixXd bc = (Eigen::MatrixXd(2,1)<<1,0).finished();
  Eigen::MatrixXd W;
  igl::BBWData data;
  igl::mosek::MosekData mosek_data;
  igl::mosek::bbw(V,F,b,bc,data,mosek_data,W);
#endif

#ifdef LIBIGL_PNG
  Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> R(1,1),G(1,1),B(1,1),A(1,1);
  R(0,0)=255; G(0,0)=0; B(0,0)=0; A(0,0)=128;
  igl::png::writePNG(R,G,B,A,"test.png");
#endif

  Eigen::MatrixXd TV;
  Eigen::MatrixXi TT,TF;
#ifdef LIBIGL_TETGEN
  igl::copyleft::tetgen::tetrahedralize(V,F,"q",TV,TT,TF);
#endif
#ifdef LIBIGL_TRIANGLE
  igl::triangle::triangulate(V.leftCols(2).eval(),Eigen::MatrixXi(),Eigen::MatrixXd(),"c",TV,TF);
#endif

#ifdef LIBIGL_XML
  igl::xml::writeDAE("test.dae",V,F);
#endif

#ifdef LIBIGL_GLFW
  igl::opengl::glfw::Viewer vr;
  vr.data().set_mesh(V,F);
#ifdef LIBIGL_IMGUI
  igl::opengl::glfw::imgui::ImGuiPlugin plugin;
  vr.plugins.push_back(&plugin);
  igl::opengl::glfw::imgui::ImGuiMenu menu;
  plugin.widgets.push_back(&menu);
#endif
  vr.launch();
#endif
}
