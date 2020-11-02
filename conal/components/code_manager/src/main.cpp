
#include <CodeManagerComponent.hpp>
#include <ComponentManager.hpp>

using namespace ::conal::code_manager; 
using namespace ::conal::framework; 

int main() {
    ComponentManager::getInstance()->registerComponent(
        "code_manager", 
        std::make_shared<CodeManagerComponent>()
    );
    return 0;
}