
`include "cypress.v"


module EiDetectie (
    input  wire clk,
    input  wire reset,
    input  wire laser_in,
    
    output reg  led_out
);

    always @(posedge clk) begin
    
        if (laser_in == 1'b0) begin
            // Laser gedetecteerd -> Zet LED UIT (0)
            led_out <= 1'b0;
        end else begin
            // Geen laser
            led_out <= 1'b1;
        end
    end

endmodule