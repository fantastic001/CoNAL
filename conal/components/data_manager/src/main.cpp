
#include <DataManagerComponent.hpp>
#include <ComponentManager.hpp>

using namespace ::conal::data_manager; 
using namespace ::conal::framework; 

int main() {
    ComponentManager::getInstance()->registerComponent(
        "data_manager", 
        std::make_shared<DataManagerComponent>()
    );
    return 0;
}