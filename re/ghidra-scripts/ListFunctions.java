import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import java.io.FileWriter;
import java.io.PrintWriter;

public class ListFunctions extends GhidraScript {
    @Override
    public void run() throws Exception {
        println("Starting ListFunctions...");
        FunctionIterator iter = currentProgram.getFunctionManager().getFunctions(true);
        int count = 0;
        try (PrintWriter out = new PrintWriter(new FileWriter("/work/functions.txt"))) {
            while (iter.hasNext()) {
                Function f = iter.next();
                out.println(String.format("%s\t%s\t%d", f.getEntryPoint(), f.getName(), f.getBody().getNumAddresses()));
                count++;
            }
        }
        println("Total functions exported: " + count);
    }
}
