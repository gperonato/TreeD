// This script reads an XYZ file and gives as output an OFF file containing the visible alpha shape facets
// It is based on the visible_alpha_shape_facets_to_OFF example file
// The compiled executable can be run in command line using as arguments the parameter alpha and the path to the XYZ file
// Contact: Giuseppe Peronato <giuseppe.peronato@uu.angstrom.se>
// Latest modification 12/03/2019


#include <chrono>  // for high_resolution_clock

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Alpha_shape_3.h>
#include <CGAL/Alpha_shape_cell_base_3.h>
#include <CGAL/Alpha_shape_vertex_base_3.h>
#include <CGAL/Delaunay_triangulation_3.h>

#include <cassert>
#include <fstream>
#include <list>

typedef CGAL::Exact_predicates_inexact_constructions_kernel               Gt;
typedef CGAL::Tag_false                                                    Alpha_cmp_tag;
//We use CGAL::Default to skip the complete declaration of base classes
typedef CGAL::Alpha_shape_vertex_base_3<Gt, CGAL::Default, Alpha_cmp_tag>   Vb;
typedef CGAL::Alpha_shape_cell_base_3<Gt, CGAL::Default, Alpha_cmp_tag>     Fb;
typedef CGAL::Triangulation_data_structure_3<Vb, Fb>                       Tds;
typedef CGAL::Delaunay_triangulation_3<Gt, Tds, CGAL::Fast_location>       Triangulation_3;
//Alpha shape with ExactAlphaComparisonTag set to False (note that the tag is also
//set to false for Vb and Fb)
typedef CGAL::Alpha_shape_3<Triangulation_3, Alpha_cmp_tag>                Alpha_shape_3;
typedef Gt::Point_3                                                       Point;

// Record start time
auto start = std::chrono::high_resolution_clock::now();

int main(int argc, char* argv[])
{
	// Initialize parameters
	double alpha = 0.5;
	char* infile = "input.xyz";

	// Check the number of parameters
	if (argc < 2) {
		// Tell the user how to run the program
		std::cerr << "Run in command line using as arguments alpha and the path to the XYZ file.\nE.g. alphashape.exe 0.75 infile.xyz\n";
		system("pause");
		return 0;
	}
	else {
		alpha = atof(argv[1]);  // alternative strtod
		infile = argv[2];
	}
  std::vector<Point> points;

//read input
  std::ifstream myfile(infile);

  // new lines will be skipped unless we stop it from happening:    
  myfile.unsetf(std::ios_base::skipws);

  // count the newlines with an algorithm specialized for counting:
  int n = std::count(
	  std::istream_iterator<char>(myfile),
	  std::istream_iterator<char>(),
	  '\n');

  std::ifstream is(infile);
  double x, y, z;
  for (int i=0;i<n;++i)
  {
	  is >> x >> y >> z;
    points.push_back( Point(x, y, z) );
  }

  std::cerr << points.size() << " points read!\n";

// set alpha shape
  std::cerr << "alpha = " << alpha << "\n";
  Alpha_shape_3 as(points.begin(), points.end(), alpha, Alpha_shape_3::REGULARIZED);
  std::cerr << as.number_of_solid_components() << " number of solid components\n";

// collect alpha-shape facets accessible from the infinity
  // marks the cells that are in the same component as the infinite vertex by flooding
  boost::unordered_set< Alpha_shape_3::Cell_handle > marked_cells;
  std::vector< Alpha_shape_3::Cell_handle > queue;
  queue.push_back( as.infinite_cell() );

  while(!queue.empty())
  {
    Alpha_shape_3::Cell_handle back = queue.back();
    queue.pop_back();

    if ( !marked_cells.insert(back).second ) continue; //already visited

    for (int i=0; i<4; ++i)
    {
      if (as.classify(Alpha_shape_3::Facet(back, i))==Alpha_shape_3::EXTERIOR &&
          marked_cells.count(back->neighbor(i))==0)
        queue.push_back( back->neighbor(i) );
    }
  }

  // filter regular facets to restrict them to those adjacent to a marked cell
  std::vector< Alpha_shape_3::Facet > regular_facets;
  as.get_alpha_shape_facets(std::back_inserter( regular_facets ), Alpha_shape_3::REGULAR );

  std::vector<Alpha_shape_3::Facet> filtered_regular_facets;
  BOOST_FOREACH(Alpha_shape_3::Facet f, regular_facets)
  {
    if ( marked_cells.count(f.first)==1 )
      filtered_regular_facets.push_back(f);
    else
    {
      f = as.mirror_facet(f);
      if ( marked_cells.count(f.first)==1 )
        filtered_regular_facets.push_back(f);
    }
  }

// dump into OFF format
  // assign an id per vertex
  boost::unordered_map< Alpha_shape_3::Vertex_handle, std::size_t> vids;
  points.clear();

  BOOST_FOREACH(Alpha_shape_3::Facet f, filtered_regular_facets)
  {
    for (int i=1;i<4; ++i)
    {
      Alpha_shape_3::Vertex_handle vh = f.first->vertex((f.second+i)%4);
      if (vids.insert( std::make_pair(vh, points.size()) ).second)
        points.push_back( vh->point() );
    }
  }

  // writing
  char* outfile = infile;
  sscanf(infile, "%[^.]", outfile);  // foo.bar => foo
  sprintf(outfile, "%s.off", outfile); // foo.txt <= foo

  std::ofstream output(outfile);
  output.precision(10);
  output << "OFF\n " << points.size() << " " << filtered_regular_facets.size() << " 0\n";
  std::copy(points.begin(), points.end(), std::ostream_iterator<Point>(output, "\n"));
  BOOST_FOREACH(const Alpha_shape_3::Facet& f, filtered_regular_facets)
  {
    output << 3;

    for (int i=0;i<3; ++i)
    {
      Alpha_shape_3::Vertex_handle vh = f.first->vertex( as.vertex_triple_index(f.second, i) );
      output << " " << vids[vh];
    }
    output << "\n";
  }

  // Record end time
  auto finish = std::chrono::high_resolution_clock::now();


  std::chrono::duration<double> elapsed = finish - start;

  std::cout << "Elapsed time: " << elapsed.count() << " s\n";


  return 0;
}
