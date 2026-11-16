import ghidra.app.script.GhidraScript;
import ghidra.program.model.data.StringDataType;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.DataIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import java.io.FileWriter;
import java.io.PrintWriter;

public class FindReferences extends GhidraScript {
    @Override
    public void run() throws Exception {
        println("Finding strings and their calling functions...");
        try (PrintWriter out = new PrintWriter(new FileWriter("/work/strings_refs.txt"))) {
            DataIterator dataIt = currentProgram.getListing().getDefinedData(true);
            while (dataIt.hasNext()) {
                Data d = dataIt.next();
                if (d.hasStringValue()) {
                    String s = d.getDefaultValueRepresentation();
                    if (s != null && (s.contains(".ADS") || s.contains(".TTM") || s.contains(".SCR") || s.contains("RESOURCE") || s.contains("ScrAntic"))) {
                        ReferenceIterator refIt = currentProgram.getReferenceManager().getReferencesTo(d.getAddress());
                        out.println("STRING [" + d.getAddress() + "]: " + s);
                        while (refIt.hasNext()) {
                            Reference r = refIt.next();
                            Function f = currentProgram.getFunctionManager().getFunctionContaining(r.getFromAddress());
                            String fnName = (f != null) ? f.getName() + " (" + f.getEntryPoint() + ")" : "none";
                            out.println("   <- ref from " + r.getFromAddress() + " in " + fnName);
                        }
                    }
                }
            }
        }
        println("Done finding string references!");
    }
}
