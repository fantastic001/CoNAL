
#include "CodeManagerComponent.hpp"
#include <ComponentManager.hpp>

using namespace ::conal::code_manager; 
using namespace ::conal::framework; 

int main() {
    ComponentManager::getInstance()->registerComponent(
        "pong", 
        std::make_shared<CodeManagerComponent>(false)
    );
    return 0;
}