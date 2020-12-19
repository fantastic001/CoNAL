
#include "CollectorComponent.hpp"
#include "ComponentManager.hpp"


using namespace ::conal::collector; 
using namespace ::conal::framework; 

int main(int argc, char** argv) {
    ComponentManager::getInstance()->registerComponent(
        "collector", 
        std::make_shared<CollectorComponent>(argc, argv)
    );
    return 0;
}