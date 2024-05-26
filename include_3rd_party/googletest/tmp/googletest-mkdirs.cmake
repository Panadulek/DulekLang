# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "E:/LocalRepo/Compilers/DulekLang/DulekLangTests/../include_3rd_party/googletest/src/googletest"
  "E:/LocalRepo/Compilers/DulekLang/DulekLangTests/../include_3rd_party/googletest/src/googletest-build"
  "E:/LocalRepo/Compilers/DulekLang/DulekLangTests/../include_3rd_party/googletest"
  "E:/LocalRepo/Compilers/DulekLang/DulekLangTests/../include_3rd_party/googletest/tmp"
  "E:/LocalRepo/Compilers/DulekLang/DulekLangTests/../include_3rd_party/googletest/src/googletest-stamp"
  "E:/LocalRepo/Compilers/DulekLang/DulekLangTests/../include_3rd_party/googletest/src"
  "E:/LocalRepo/Compilers/DulekLang/DulekLangTests/../include_3rd_party/googletest/src/googletest-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/LocalRepo/Compilers/DulekLang/DulekLangTests/../include_3rd_party/googletest/src/googletest-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/LocalRepo/Compilers/DulekLang/DulekLangTests/../include_3rd_party/googletest/src/googletest-stamp${cfgdir}") # cfgdir has leading slash
endif()
