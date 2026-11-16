import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.mem.Memory;
import java.io.FileWriter;
import java.io.PrintWriter;

public class InspectDataPointers extends GhidraScript {
    @Override
    public void run() throws Exception {
        Memory mem = currentProgram.getMemory();
        try (PrintWriter out = new PrintWriter(new FileWriter("/work/data_inspection.txt"))) {
            out.println("=== Search memory for 16-bit values 0x1681 (ACTIVITY.ADS) and 0x1cbc (RESOURCE.MAP) ===");
            AddressIterator it = mem.getAddresses(true);
            int matchCount = 0;
            while (it.hasNext()) {
                Address a = it.next();
                try {
                    short val = mem.getShort(a);
                    if (val == (short)0x1681 || val == (short)0x1cbc || val == (short)0x16b4) {
                        String target = (val == (short)0x1681) ? "ACTIVITY.ADS" : (val == (short)0x1cbc ? "RESOURCE.MAP" : "JOHNNY.ADS");
                        Instruction inst = currentProgram.getListing().getInstructionAt(a);
                        Function f = currentProgram.getFunctionManager().getFunctionContaining(a);
                        String fn = (f != null) ? f.getName() + " (" + f.getEntryPoint() + ")" : "none";
                        out.println(String.format("Match at %s (val=0x%04x -> %s) in fn: %s, inst: %s", a, val & 0xffff, target, fn, inst));
                        matchCount++;
                    }
                } catch (Exception ignored) {}
            }
            out.println("Total matches: " + matchCount);
        }
    }
}
