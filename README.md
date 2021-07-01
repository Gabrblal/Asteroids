# Asteroids
Implementation of the game Asteroids done in SDL, with a goal of implementation from scratch broad and narrow phase collision detection.

Current status:
- :arrows_counterclockwise: General structure.
    - :arrows_counterclockwise: Controller / View / Model structure.
    - :arrows_counterclockwise: Polygon structure implementation.
    - :black_square_button: [Quad Tree](https://en.wikipedia.org/wiki/Quadtree) implementation.
    - :arrows_counterclockwise: Vector2 arithmetic (including projection, rotation).
- :black_square_button: Collision Detection
    - :arrows_counterclockwise: Narrow phase polygon collision detection using seperating axis theorem.
    - :black_square_button: Broad phase polygon organisation / collision detection using quad trees.
    - :black_square_button: Continuous collision detection.
- :black_square_button: Simulation.
    - :black_square_button: Asteroid.
        - :black_square_button: Asteroid simulation with position, linear and rotational velocity, acceleration and inertia.
        - :black_square_button: Splitting into two Asteroids.
        - :black_square_button: Random generation.
        - :black_square_button: Quiet sound on collision.
        - :black_square_button: Elasticity.
    - :black_square_button: Player.
        - :black_square_button: Polygon structure implementation.
        - :black_square_button: Projectiles (sound on fire) (raycasting?).
        - :black_square_button: Health.
    - :black_square_button: World.
        - :black_square_button: Game boundaries.
        - :black_square_button: Music.
        - :black_square_button: Background?
        - :black_square_button: Gravity!
    - :black_square_button: Heads up display.
        - :black_square_button: Health.
