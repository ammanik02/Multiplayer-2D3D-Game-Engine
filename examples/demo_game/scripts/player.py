import engine

class Player:
    def __init__(self, entity):
        self.entity = entity
        self.speed = 300.0

    def update(self, dt):
        dx = 0
        dy = 0
        
        # Simple WASD
        if engine.Input.is_key_down(4): dx -= self.speed * dt # A
        if engine.Input.is_key_down(7): dx += self.speed * dt # D
        if engine.Input.is_key_down(26): dy -= self.speed * dt # W
        if engine.Input.is_key_down(22): dy += self.speed * dt # S
        
        if dx != 0 or dy != 0:
            self.entity.move(dx, dy)
