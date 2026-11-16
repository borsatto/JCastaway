import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.mem.Memory;
import java.io.FileWriter;
import java.io.PrintWriter;

public class FindTimerSlots extends GhidraScript {
    @Override
    public void run() throws Exception {
        Memory mem = currentProgram.getMemory();
        try (PrintWriter out = new PrintWriter(new FileWriter("/work/timer_refs.txt"))) {
            out.println("=== Search memory for references to timer table 0x45ac and 0x45de ===");
            AddressIterator it = mem.getAddresses(true);
            while (it.hasNext()) {
                Address a = it.next();
                try {
                    short val = mem.getShort(a);
                    if (val == (short)0x45ac || val == (short)0x45de || val == (short)0x45a8) {
                        Function f = currentProgram.getFunctionManager().getFunctionContaining(a);
                        Instruction inst = currentProgram.getListing().getInstructionAt(a);
                        out.println(String.format("Offset 0x%04x referenced at %s in fn: %s, inst: %s",
                            val & 0xffff, a, (f != null ? f.getName() + " (" + f.getEntryPoint() + ")" : "none"), inst));
                    }
                } catch (Exception ignored) {}
            }
        }
    }
}
