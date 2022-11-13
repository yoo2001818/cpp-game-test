# AABB Physics

So far, I’ve been trying to implement a “proper” physics engine using AABB collision. Since the target is a platformer arcade game, a simple AABB physics engine would suffice. I’ve been trying to apply impulse response, etc, to properly implement physics, without applying any angular momentum (because AABB rectangle can’t rotate).

However, since I’ve pretty much lost interest in this, and it’s not possible to follow the reference book through without implementing angular momentum and etc, it’d be wiser to simply implement arcade physics engine without trying to follow the book through.

So far, what I’ve implemented can be all reused - Each object has acceleration, velocity, and position. The world may apply force to the object to implement gravity, wind, drag, keyboard movement (thrusting).

On each step, each object is applied velocity from the applied force, and position from the velocity.

After moving the object around, we determine if each object has collided with other object.

During this, we can determine the intersection of the object and thus determine the normal of the collision.

The normal of the collision is pretty hard to determine, but we know what we should NOT to calculate this. Ouch. I’ve tried calculating this using previous position and the current position, i.e. velocity, But this doesn’t work at all because, if the object is thrusting (moving along X) on the ground, since the object has bigger velocity on the X axis, it registers the collision with the ground as horizontal collision!

Obviously this is catastrophic. We need to find a sure-fire way to determine the normal of the collision.

There can be numerous ways to the collision to occur - let’s try enumerating them.

1. Edge-edge collision. The object’s edge is completely inside the ground. This is the most unambiguous situation - we just have to push the edge to the outside.
2. Vertex-vertex collision. The object’s single vertex is inside the ground. This is ambiguous - we don’t know which direction the object has collided!
3. Edge-rectangle collision. The object has completely penetrated the ground. The ground has to push the object further than the intersection.
4. Rectangle-rectangle collision. The object is inside the ground. This is truly undecidable - which direction should we push the object away?

Mostly, we have to care about 1st and 2nd case. 1st case is pretty straightforward - however 2nd case causes all the problem.

For example, we’ve been experiencing a lot of issues because of the tile-map - The ground is not a whole rectangle.

Each object can be standing on multiple ground objects. If we make the collision algorithm poorly, one of the ground may decide that collision has happened in the WRONG direction and act accordingly, causing a lot of mayhem on the world.

The physics book I’ve been reading, Physics for Game Developers, proposes using a “buffer region” to determine the direction no matter what.

Each object has buffer region, that tries to register a collision if other object gets inside the buffer region. If the objects completely penetrate each other, the physics step gets rolled back, and retries the physics step with halved timescale.

In a way, this implements a robust collision detection… by never allowing collision. This will work in most cases, however we need to make the object slightly smaller than a tile to allow the object to get inside the tile.

Is there a better way? I think so, but nothing will be easier than this - we never have to cancel the movement because of the buffer region, furthermore it is laughingly trivial to determine the collision edge. However the “arcade physics” probably won’t be implemented this way because old devices couldn’t afford to retrying with lowered timescale.

I’ve looked at NES Super Mario Bros 1’s collision check flowchart here - [https://twitter.com/dansalvato/status/1469576958241492993/photo/1](https://twitter.com/dansalvato/status/1469576958241492993/photo/1) - and it’s pretty much clear they relied on a lot of heuristics - the collision is specifically tailored to the player’s platforming action.

I think it’s safe to say that AABB is NOT a good shape to determine the collision. Seriously, if the player is a capsule, it’d be much, much easier.

Well, at this point, I have to determine this - The physics engine should be tailored to platforming action, since the game is a…. platformer. While I COULD implement this using buffer region, there will be a lot of tailoring to make it fit to the game, and the buffer region will make everything float, like an air hockey. This will be unacceptable if the buffer region is too large.

Ok, with that being concluded, let’s implement platformer-game-grade physics engine!
