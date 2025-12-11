import engine

class PlayerController:
    def __init__(self, entity):
        self.entity = entity
        print(f"PlayerController attached to entity {entity}")

    def update(self, dt):
        speed = 200.0 * dt
        
        # Reset velocity (optional, depends on if we want physics or direct movement)
        # Here we just modify position directly via move() as per requested API example
        # But wait, the requested API was move(dx, dy)
        
        dx = 0
        dy = 0
        
        if engine.Input.is_key_down(4): # SDL_SCANCODE_A
            dx -= speed
        if engine.Input.is_key_down(7): # SDL_SCANCODE_D
            dx += speed
        if engine.Input.is_key_down(26): # SDL_SCANCODE_W
            dy -= speed
        if engine.Input.is_key_down(22): # SDL_SCANCODE_S
            dy += speed
            
        if dx != 0 or dy != 0:
            self.entity.move(dx, dy)
