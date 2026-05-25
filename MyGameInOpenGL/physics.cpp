#include "physics.h"

void Physics::update(Player& player, Entity& entity, float dt) {


	resolveGroundCollision(player, entity);

	applyGravity(player, dt);

}

void Physics::applyGravity(Player& player, float dt) {
	// apply gravity over time if player is in the air (not grounded)
	if (!player.isGrounded) {
		player.rb.Velocity += gravity * dt;
	}
	player.transform.Position += player.rb.Velocity * dt;
}

void Physics::resolveGroundCollision(Player& player, Entity& ground){
	// Basic collision detection
	float playerHalfHeight = player.transform.Size.y * 0.5f;
	float playerBottom = player.transform.Position.y - playerHalfHeight;

	float groundHalfHeight = ground.transform.Size.y * 0.5f;
	float groundTop = ground.transform.Size.y + groundHalfHeight;

	// Not falling
	if (playerBottom <= groundTop) {
		// ensure the point on the player that's on bottom/collides with ground
		// is the bottom, not the center
		player.transform.Position.y = playerHalfHeight + groundTop;
		if (player.rb.Velocity.y < 0.0f) {
			// bounce
			player.rb.Velocity.y *= -restitution; 
			// Remove any potential jitter/infinite bounce near ground
			if (player.rb.Velocity.y < 0.1f) {
				player.rb.Velocity.y = 0.0f;
			}
		}
		player.isGrounded = true;
	}
	// Falling
	else {
		player.isGrounded = false;
	}
}