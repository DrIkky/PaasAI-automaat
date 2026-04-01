
`include "cypress.v"

module EiDetectie (
    input  wire clk,
    input  wire reset,     // Deze pin gaan we vanuit C aansturen!
    input  wire laser_in,
    
    output reg  led_out    // 1 = Klaar/Geen ei, 0 = Ei gedetecteerd (Gelatched!)
);

    always @(posedge clk) begin
        if (reset == 1'b1) begin
            // De PSoC geeft een reset: wis het geheugen en zet signaal weer Hoog (1)
            led_out <= 1'b1;
        end else if (laser_in == 1'b0) begin
            // Laser is onderbroken! Trek het signaal Laag (0) en HOUD HET LAAG
            led_out <= 1'b0;
        end
        // Als er geen reset is en geen nieuw ei, onthoudt hij automatisch zijn staat.
    end

endmodule