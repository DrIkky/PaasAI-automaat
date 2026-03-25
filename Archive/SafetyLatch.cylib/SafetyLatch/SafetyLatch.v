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

module EggDetector (
    input  wire clk,         // Fast system clock
    input  wire reset,       // Button to turn the LED back off
    input  wire laser_in,    // The signal from your laser detector
    
    output reg  led_out      // 1-bit output to an LED pin
);

    always @(posedge clk) begin
        if (reset == 1'b1) begin
            led_out <= 1'b0;       // Reset: turn LED off
        end else if (laser_in == 1'b1) begin
            // Assuming laser_in goes HIGH (1) when the beam is broken:
            led_out <= 1'b1;       // Latch: turn LED on and keep it on
        end
    end

endmodule