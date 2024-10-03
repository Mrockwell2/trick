package trick.common;

import trick.common.SimulationInterface;

public class TestUtils {
    public static String getTrickHome() {
        String cwd = System.getProperty("user.dir");
        int trick_index = cwd.indexOf("/trick/trick_source/java");

        return cwd.substring(0, trick_index + 6);
    }

    public static boolean compileTestSim(String sim_name) {
        String sim_path = getTrickHome() + "/test/" + sim_name;
        
        SimulationInterface.cleanSim(sim_path);
        return SimulationInterface.compileSim(sim_path);
    }
}