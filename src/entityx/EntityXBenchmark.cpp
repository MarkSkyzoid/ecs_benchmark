#include "EntityXBenchmark.h"

namespace entityx1_benchmark {

void MovementSystem::update(entityx::EntityManager &es,
                            entityx::EventManager &events,
                            entityx::TimeDelta dt) {
  Component<PositionComponent> position;
  Component<DirectionComponent> direction;

  for (auto entity : es.entities_with_components(position, direction)) {
    position->x += direction->x * dt;
    position->y += direction->y * dt;
  }
}

void ComflabSystem::update(entityx::EntityManager &es,
                           entityx::EventManager &events,
                           entityx::TimeDelta dt) {
  Component<ComflabulationComponent> comflab;

  for (auto entity : es.entities_with_components(comflab)) {
    comflab->thingy *= 1.000001f;
    comflab->mingy = !comflab->mingy;
    comflab->dingy++;
    // comflab.stringy = std::to_string(comflab.dingy);
  }
}

int MoreComplexSystem::random(int min, int max) {
  // Seed with a real random value, if available
  static std::random_device r;

  // Choose a random mean between min and max
  static std::default_random_engine e1(r());

  std::uniform_int_distribution<int> uniform_dist(min, max);

  return uniform_dist(e1);
}

void MoreComplexSystem::update(entityx::EntityManager &es,
                               entityx::EventManager &events,
                               entityx::TimeDelta dt) {
  Component<PositionComponent> position;
  Component<DirectionComponent> direction;
  Component<ComflabulationComponent> comflab;

  for (auto entity : es.entities_with_components(comflab, direction, comflab)) {
    if (comflab) {
      std::vector<double> vec;
      for (size_t i = 0; i < comflab->dingy && i < 100; i++) {
        vec.push_back(i * comflab->thingy);
      }

      int sum = std::accumulate(std::begin(vec), std::end(vec), 0.0);
      int product = std::accumulate(std::begin(vec), std::end(vec), 1,
                                    std::multiplies<double>());

      comflab->stringy = std::to_string(comflab->dingy);

      if (position && direction && comflab->dingy % 10000 == 0) {
        if (position->x > position->y) {
          direction->x = random(0, 5);
          direction->y = random(0, 10);
        } else {
          direction->x = random(0, 10);
          direction->y = random(0, 5);
        }
      }
    }
  }
}

Application::Application(bool addmorecomplexsystem)
    : addmorecomplexsystem_(addmorecomplexsystem) {
  this->systems.add<MovementSystem>();
  this->systems.add<ComflabSystem>();
  if (this->addmorecomplexsystem_) {
    this->systems.add<MoreComplexSystem>();
  }

  this->systems.configure();
}

void Application::update(TimeDelta dt) {
  this->systems.update<MovementSystem>(dt);
  this->systems.update<ComflabSystem>(dt);
  if (this->addmorecomplexsystem_) {
    this->systems.update<MoreComplexSystem>(dt);
  }
}

} // namespace entityx1_benchmark