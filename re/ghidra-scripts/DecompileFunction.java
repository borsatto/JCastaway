import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import java.io.FileWriter;
import java.io.PrintWriter;

public class DecompileFunction extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] targetFns = new String[] { "1018:1771", "1008:0fcd", "1008:034f" };
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        try (PrintWriter out = new PrintWriter(new FileWriter("/work/decompiled_functions.c"))) {
            for (String addrStr : targetFns) {
                Address a = currentAddress.getAddressSpace().getAddress(addrStr);
                Function f = currentProgram.getFunctionManager().getFunctionAt(a);
                if (f == null) {
                    f = currentProgram.getFunctionManager().getFunctionContaining(a);
                }
                out.println("// ==========================================================================");
                out.println("// Function at " + addrStr + " -> " + (f != null ? f.getName() : "null"));
                out.println("// ==========================================================================");
                if (f != null) {
                    DecompileResults res = decomp.decompileFunction(f, 60, monitor);
                    if (res != null && res.getDecompiledFunction() != null) {
                        out.println(res.getDecompiledFunction().getC());
                    } else {
                        out.println("// Decompilation failed: " + (res != null ? res.getErrorMessage() : "unknown"));
                    }
                }
            }
        }
        println("Decompilation complete!");
    }
}
