package dev.kamilklecha.autoswitchmobile;

import java.io.Serializable;
import java.sql.Time;
import java.util.Date;

public class Packet implements Serializable {
    public
    String Exec;
    String Action;
    String TimeMode;
    Integer seconds;
    Time time;
    Date date;
    Boolean force;
}
