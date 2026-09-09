### Software Architecture & Control Logic

1. **Sensor Processing Loop:** The firmware continuously samples three ultrasonic rangefinders (Front, Left, Right) using tight timing timeouts ($10\text{ ms}$) to ensure high execution loops without blocking execution.
2. **Obstacle Override Strategy:** If the front sensor registers an obstacle under $25\text{ cm}$, the PD controller suspends and the vehicle enters an in-place pivot turn toward whichever side offers greater clearance.
3. **Closed-Loop PD Corridor Centering:**
   * **Proportional ($K_p$):** Corrects lateral displacement relative to corridor walls.
   * **Derivative ($K_d$):** Dampens rapid oscillations and prevents high-speed weaving.
   * **Differential PWM:** Modulates left and right wheel speeds independently to maintain high velocity through sweeping corners without losing traction.