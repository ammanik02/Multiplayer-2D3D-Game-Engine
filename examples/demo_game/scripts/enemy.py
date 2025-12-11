import engine

class Enemy:
    def __init__(self, entity):
        self.entity = entity
        self.direction = 1
        self.timer = 0
        self.speed = 100.0

    def update(self, dt):
        # Patrol back and forth
        self.entity.move(self.speed * self.direction * dt, 0)
        
        self.timer += dt
        if self.timer > 2.0:
            self.direction *= -1
            self.timer = 0
