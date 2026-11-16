import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import java.io.FileWriter;
import java.io.PrintWriter;

public class DecompileTimerRegister extends GhidraScript {
    @Override
    public void run() throws Exception {
        Address a = currentAddress.getAddressSpace().getAddress("1040:079e");
        Function f = currentProgram.getFunctionManager().getFunctionAt(a);
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        try (PrintWriter out = new PrintWriter(new FileWriter("/work/timer_callbacks.txt"))) {
            out.println("=== Decompilation of Timer Register (1040:079e) ===");
            if (f != null) {
                DecompileResults res = decomp.decompileFunction(f, 60, monitor);
                if (res != null && res.getDecompiledFunction() != null) {
                    out.println(res.getDecompiledFunction().getC());
                }
            }
            out.println("\n=== Callers of 1040:079e ===");
            ReferenceIterator refIt = currentProgram.getReferenceManager().getReferencesTo(a);
            while (refIt.hasNext()) {
                Reference r = refIt.next();
                Function caller = currentProgram.getFunctionManager().getFunctionContaining(r.getFromAddress());
                out.println("Caller at " + r.getFromAddress() + " in " + (caller != null ? caller.getName() + " (" + caller.getEntryPoint() + ")" : "none"));
            }
        }
    }
}
