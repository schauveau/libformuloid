
namespace fmd {

std::string pretty(const std::string &str) ;

std::string pretty(const fmd::internal::node_root &node) ;

/// \brief produce a pretty version of a formuloid formula
template <IsFormuloid F>
std::string pp(const F &f)
{
  return pretty(fmd::get_node(f));
}

};
