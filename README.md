# Asteroids
Implementation of the game Asteroids done in SDL, with a goal of implementation from scratch broad and narrow phase collision detection.

Current status:
- :arrows_counterclockwise: General structure.
    - :arrows_counterclockwise: Model
        - :white_check_mark: General structure, creation, updating, deleting.
        - :black_square_button: Integrating with simulation.
    - :arrows_counterclockwise: View / Model structure.
        - :white_check_mark: General structure, creation, updating, deleting.
        - :arrows_counterclockwise: ViewPort.
        - :arrows_counterclockwise: View panning and zooming.
        - :black_square_button: Debugging grid.
    - :arrows_counterclockwise: Controller
        - :white_check_mark: General structure, creation, updating, deleting.
        - :white_check_mark: Controller event handling structure.
        - :black_square_button: Keyboard event handling (to be determined).
        - :black_square_button: Mouse event handling.
    - :white_check_mark: Threading.
    - :arrows_counterclockwise: Polygon structure implementation.
        - :arrows_counterclockwise: Creation, deletion.
        - :black_square_button: Random generation of concave polygons.
        - :black_square_button: Rendering.
    - :black_square_button: [Quad Tree](https://en.wikipedia.org/wiki/Quadtree) implementation.
    - :arrows_counterclockwise: Vector2.
        - :black_square_button: Structure definition (cartesian, polar?)
        - :black_square_button: Elementwise :heavy_plus_sign: :heavy_minus_sign: :heavy_multiplication_x: :heavy_division_sign:.
        - :black_square_button: Vector dot product, projection, rotation and unit vector.
- :black_square_button: Polygon Collision Detection
    - :arrows_counterclockwise: Using axially aligned bounding boxes (AABB) (first fast check for collision for speed).
    - :arrows_counterclockwise: Using seperating axis theorem (second slower check for precision).
    - :black_square_button: Broad phase polygon organisation / collision detection using quad trees.
    - :black_square_button: Continuous collision detection.
- :black_square_button: Simulation.
    - :black_square_button: Determine from time elapsed (no continuous loops).
    - :black_square_button: Asteroid.
        - :black_square_button: Asteroid simulation with position, linear and rotational velocity, acceleration and inertia.
        - :black_square_button: Correct inertial collision response depending on distance from centre of gravity to collision.
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
