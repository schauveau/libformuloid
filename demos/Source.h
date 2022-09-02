
std::vector<std::string> source_lines ; 

void load_source_file(const char *filename)
{
    std::ifstream fin(filename) ;
    if (!fin.good()) {
      std::cerr << "WARNING: Source file " << filename << " not found! Code will not be displayed\n" ;
    }
    std::string line;
    source_lines.push_back(""); // Dummy first line so that numbering starts at 1
    while(getline(fin,line)){
      source_lines.push_back(line);
    }
}

// Append the lines from the source file to array A
// with some filters.
void append_from_source(std::vector<std::string> & A,
                        size_t begin,
                        size_t end,
                        std::vector<std::string> &discard_keywords,
                        size_t drop_spaces)
{
  begin = std::max(begin, size_t(0) );
  end   = std::min(end, source_lines.size() );
  bool last_was_empty = true; 
  // TODO: Use first line to remove indentation
  for ( size_t i = begin ; i<=end ; i++ ) {
    std::string & line = source_lines[i] ;

    // Discard lignes containing any of the specified keywords
    bool discard = false;
    for ( auto & keyword : discard_keywords) {
      if ( line.find(keyword) != std::string::npos ) {
        discard = true;
        continue;
      }
    }    
    if (discard)
      continue;
      
    // Discard repeating empty lines
    auto first_char = line.find_first_not_of(" \t") ;
    bool is_empty = ( first_char == std::string::npos );
    if (is_empty && last_was_empty)
      continue;
   
    if ( !is_empty ) {
      line = line.substr(std::min(drop_spaces,first_char)) ;
    }
        
    A.push_back(line) ;
    last_was_empty = is_empty;
  }  
}
