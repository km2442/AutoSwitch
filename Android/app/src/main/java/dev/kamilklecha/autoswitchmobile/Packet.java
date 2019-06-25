package dev.kamilklecha.autoswitchmobile;

import java.io.Serializable;
import java.util.Calendar;


public class Packet implements Serializable {
    public
    String Exec;
    String Action;
    String TimeMode;
    Integer seconds;
    Calendar dt;
    Boolean force;
    SettingsPC PCSettings;
}

class SettingsPC implements Serializable {
    public
    int Language;
    int Theme;
    boolean TrayVisible;
    boolean Statistics;
    boolean MultiInstance;
    boolean Logs;
    boolean HideWarning;
    boolean TestMode;
}
