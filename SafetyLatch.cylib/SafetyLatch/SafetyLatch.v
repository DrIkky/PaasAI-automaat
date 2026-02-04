
//`#start header` -- edit after this line, do not edit this line
// ========================================
//
// Copyright YOUR COMPANY, THE YEAR
// All Rights Reserved
// UNPUBLISHED, LICENSED SOFTWARE.
//
// CONFIDENTIAL AND PROPRIETARY INFORMATION
// WHICH IS THE PROPERTY OF your company.
//
// ========================================
`include "cypress.v"
//`#end` -- edit above this line, do not edit this line
// Generated on 02/04/2026 at 19:56
// Component: SafetyLatch
module SafetyLatch (
    // Inputs
    input  wire clk,
    input  wire rst_n,  // AANGEPAST: 1 = Resetten (Veilig maken)
    input  wire panic_btn,  // 1 = ALARM!
    
    // PWM signalen van de software
    input  wire pwm1_in,
    input  wire pwm2_in,
    input  wire pwm3_in,
    input  wire pwm4_in,

    // Outputs
    output reg servo1_out,
    output reg servo2_out,
    output reg servo3_out,
    output reg servo4_out,
    output reg status_led
);

    // --- DEFINITIES ---
    localparam STATE_SAFE  = 1'b0;
    localparam STATE_ALARM = 1'b1;

    reg current_state;

    // --- STATE MACHINE (Het Geheugen) ---
    // Reageert op de klok OF als je direct op reset drukt
    always @(posedge clk or posedge rst_n) begin
        if (rst_n == 1'b1) begin
            // Als Reset wordt ingedrukt: Direct naar VEILIG
            current_state <= STATE_SAFE;
        end else begin
            // Normaal gedrag (Kijken naar Panic knop)
            case (current_state)
                STATE_SAFE: begin
                    if (panic_btn == 1'b1) begin
                        current_state <= STATE_ALARM; // Spring naar ALARM
                    end
                end

                STATE_ALARM: begin
                    // Blijf hier voor altijd hangen!
                    // We gaan pas weg als 'reset_btn' hierboven wordt ingedrukt.
                    current_state <= STATE_ALARM;
                end
            endcase
        end
    end

    // --- OUTPUT LOGICA ---
    // Bepaalt wat de pinnen doen op basis van de status
    always @(current_state or pwm1_in or pwm2_in or pwm3_in or pwm4_in) begin
        if (current_state == STATE_SAFE) begin
            // VEILIG: Geef signalen door en zet lampje AAN
            servo1_out = pwm1_in;
            servo2_out = pwm2_in;
            servo3_out = pwm3_in;
            servo4_out = pwm4_in;
            status_led = 1'b0;
        end else begin
            // ALARM: Blokkeer alles en zet lampje UIT
            servo1_out = 1'b0;
            servo2_out = 1'b0;
            servo3_out = 1'b0;
            servo4_out = 1'b0;
            status_led = 1'b1;
        end
    end

endmodule