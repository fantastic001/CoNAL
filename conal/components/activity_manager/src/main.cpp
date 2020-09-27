
#include <ActivityManagerComponent.hpp>
#include <ComponentManager.hpp>

using namespace ::conal::activity_manager; 
using namespace ::conal::framework; 

int main() {
    ComponentManager::getInstance()->registerComponent(
        "activity_manager", 
        std::make_shared<ActivityManagerComponent>()
    );
    return 0;
}