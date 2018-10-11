file(REMOVE_RECURSE
  "parser.pdb"
  "parser"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/parser.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
