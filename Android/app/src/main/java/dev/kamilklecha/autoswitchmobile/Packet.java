package dev.kamilklecha.autoswitchmobile;

import java.io.Serializable;
import java.util.Calendar;


public class Packet implements Serializable {
    public
    String Exec;
    String Action;
    String TimeMode;
    Integer seconds;
    Calendar dt = Calendar.getInstance();
    Boolean force;
}
