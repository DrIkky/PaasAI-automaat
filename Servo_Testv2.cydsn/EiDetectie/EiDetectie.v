//`#start header` -- edit after this line, do not edit this line
// ========================================
`include "cypress.v"
//`#end` -- edit above this line, do not edit this line

module EiDetectie (
    input  wire clk,
    input  wire reset,
    input  wire laser_in,
    
    output reg  led_out
);

    always @(posedge clk) begin
        // In jouw Arduino code: LOW (0) betekent "Laser gedetecteerd!"
        if (laser_in == 1'b0) begin
            // Laser gedetecteerd -> Zet LED UIT (0)
            led_out <= 1'b0;
        end else begin
            // Geen laser (pin wordt door de Pull-Up hoog gehouden) -> Zet LED AAN (1)
            led_out <= 1'b1;
        end
    end

endmodule