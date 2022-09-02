#include <string>
#include <string_view>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <stack>
#include <cassert>

#include <fmd/fmd.h>

namespace fmd
{

//std::string test = "(-> @ (let ((AAA_1 (+ (* @ @) 1)) (BBB_1 (+ @ 1)) (CCC_1 (+ @ 2))) (+ (+ AAA_1 (* BBB_1 AAA_1)) (/ CCC_1 AAA_1))))" ;
//std::string test = "(+ aaaaaaaa bbbbbbbbb cccccccccc ddddddddd)";

/// \brief a simple pretty printer for formuloid formulas
class pretty_printer
{

  std::string_view view;

  // limit after which spliting lines should be considered
  size_t max_per_line{30} ;

  // Don't bother to split a line for a small gain
  size_t split_gain{10} ; 

  struct Token
  {       
    std::string_view str;    
    // The length in tokens of the sub-expression started by this token.
    // For a token "(", this is the number of token up to the ")" so at least 2. 
    // For any other token this is 1.
    size_t length;

    // A value of zero indicates that the sub-expression is rendered 
    // on multiple lines.
    // Else, the subexpression is rendered on a single line using
    // that many characters.
    size_t line_size{0};
    
    Token( std::string_view &view,
           size_t start,
           size_t size,
           size_t len=1) :
      str(view.substr(start,size)),
      length(len)
    {
    }
  };
  

  std::vector<Token> tokens;

  bool tokenize()
  {    
    // Decompose the view into tokens
    //   - strings are double-quoted using \ as escape character.
    //   - '(' and ')' are tokens.
    //   - ' ' and '\n' are token separators.
    //   - any other sequence of characters is a token.
    //

    size_t len=0;
    bool in_string=false;
    bool after_escape=false;

    std::stack<size_t> open;
      
    for ( size_t pos=0; pos<view.length() ; ++pos )
    {
      char c = view[pos] ;

      if (in_string)
      {
        len++;
        if (after_escape) {        
          after_escape=false;
        } else {
          if (c=='"') {
            in_string=false;
            tokens.emplace_back(view,pos-len+1,len) ;
          } else if (c=='\\') {
            after_escape=true;
          }
        }
      }
      else if ( c=='(' )
      {
        if (len>0) {
          tokens.emplace_back(view,pos-len,len);
          len=0;
        }
        open.push(tokens.size());
        tokens.emplace_back(view,pos,1);
      }
      else if ( c==')' )
      {        
        if (len>0) {
          tokens.emplace_back(view,pos-len,len);
          len=0;
        }
        if (open.empty()) 
          return false; // unbalanced 
        size_t start = open.top();
        size_t end   = tokens.size();
        open.pop();
        tokens[start].length = end-start+1 ;
        tokens.emplace_back(view,pos,1);
      }
      else if (c==' '|| c=='\n')
      {
        if (len>0) {
          tokens.emplace_back(view,pos-len,len);
          len=0;
        }
      }
      else
      {   
        len++; 
      }
    }
    if (len>0) {
      tokens.emplace_back(view,view.length()-len,len);
      len=0;
    }
    
    if (!open.empty())
      return false;
    
    return true;
  }

  void
  process(size_t pos)
  {
    Token & tok = tokens[pos];

    // Simple case: the expression is composed of a single token
    if (tok.length==1) {      
      tok.line_size = tok.str.size();
      return ;
    }

    size_t max_child_size=0;
    size_t line_size=2; // for the opening and closing () 
    int count = 0;
    size_t offset=1 ;
    while ( offset < tok.length-1 ) {
      size_t child_pos = pos+offset;
      Token & child_tok = tokens[child_pos];

      process(child_pos);

      if (child_tok.line_size) {
        if (line_size!=0) {
          max_child_size = std::max( max_child_size, child_tok.line_size );
          line_size += child_tok.line_size;
          if (count>0) {
            line_size++; // will need a space
          }
        }
      } else {
        line_size = 0; 
      }
      
      count++;
      offset += child_tok.length;
    }
    assert(offset+1==tok.length) ; 

    if (line_size!=0) {      
      if (count>1) { 
        if (line_size > max_per_line) {
          // The line is too long.
          // Split it if the gain is enough.
          if ( line_size - max_child_size > split_gain ) {
            line_size=0;
          }
        }
      }
    }

    tok.line_size = line_size;
    return ;    
  }


  
  void
  render(std::ostream &s, size_t pos, size_t indent)
  {
    Token & tok = tokens[pos];

    if (tok.length==1) {
      s << tok.str;
      return;
    } 

    s << "(";
    if (tok.line_size>0)
    {
      // Draw everything up to the closing ')' on one line
      for (size_t i=pos+1; i<pos+tok.length-1 ;i++) {
        bool no_space = ( tokens[i-1].str=="(" || tokens[i].str==")") ;
        if (!no_space) {
          s << " ";
        }
        s << tokens[i].str;
      }
    }
    else
    {
      size_t k = 1;      
      if ( tokens[pos+k].str == "(") {
        // A double (( as in a 'let'.
        // All children should be aligned identically
        //
        //   (let
        //     ((AAAA ....) 
        //      (BBBB ....)
        //      (CCCC ....))
        //     (+ AAA BBB CCC))
        //
        s<<"";       
        indent+=1;
        render(s,pos+k,indent) ;
      } else {
        // A lisp command. The first child is the 
        // command name and the rest are the arguments:
        // 
        //   (foobar
        //     (+ 3 @)
        //     444
        //     (* @ @))
        //            
        indent+=1;
        render(s,pos+k,indent) ;
        indent+=1;
      }
      k += tokens[pos+k].length;
      while (k<tokens[pos].length-1) {
        s << "\n" << std::setw(indent-1) << ' ' ;  
        render(s,pos+k,indent) ;
        k += tokens[pos+k].length;
      }
    }
    s << ")";
    return ;        
  }
  
public:

  /// \c Apply the pretty printer to a std::string
  std::string
  run(const std::string &str)
  {
    view=str ;
    tokens.clear();
    
    bool balanced = tokenize();
    
    if (!balanced) {
      return str ;
    }

    if (tokens.size()==0)
      return "";

    // A formula should be a single token
    // or should start with '(' and end with a matching token.
    // In any case, the length at the first token should be the
    // full length.
    if (tokens[0].length != tokens.size() )
      return str;

    this->process(0);
    std::ostringstream s ;
    this->render(s,0,0);
    return s.str();
  }

};

/// \brief produce a pretty version of a formuloid formula
std::string pretty(const std::string &str)
{
  pretty_printer printer;
  return printer.run(str);
}

/// \private
std::string pretty(const fmd::internal::node_root &node)
{
  std::ostringstream s;
  node.write_formula(s);
  return fmd::pretty(s.str());  
}




}


