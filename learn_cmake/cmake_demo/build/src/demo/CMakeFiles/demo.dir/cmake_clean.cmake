FILE(REMOVE_RECURSE
  "CMakeFiles/demo.dir/demo_main.c.o"
  "../../../bin/demo.pdb"
  "../../../bin/demo"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang C)
  INCLUDE(CMakeFiles/demo.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
