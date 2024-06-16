

#include "AstElement.hpp"
#include <memory>
template<typename Destiny>
Destiny* ast_element_cast(AstElement*);

template<typename Destiny>
Destiny* ast_unique_element_cast(std::unique_ptr<AstElement>&);