
#include "CodeManagerComponent.hpp"
#include <ComponentManager.hpp>

using namespace ::conal::code_manager; 
using namespace ::conal::framework; 

int main() {
    ComponentManager::getInstance()->registerComponent(
        "ping", 
        std::make_shared<CodeManagerComponent>(true)
    );
    return 0;
}