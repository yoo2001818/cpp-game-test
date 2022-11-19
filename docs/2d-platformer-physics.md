# 2D Platformer Physics

In the previous document, I’ve determined that physics engine doesn’t actually work on platformer games, so I need to build a platformer-grade physics engine.

But how am I going to build it? Well, it should be pretty simple - after all, these were done in NES / SNES age - nothing too complex would be needed.

It's actually very simple to implement this, by ignoring certain area of vertical
region. Since Y axis is so important, we can implement Y axis fully - if the top,
bottom edge collides with the object, we can push the player in that direction.

However, it's the left/right collision that causes the problem. By ignore some
pixels at the top/bottom, we can completely ignore the collision problem.

It should be noted that pushing the entity back will still cause the physics
engine to jank - pushing it back should be performed after all collision checks
are complete.

