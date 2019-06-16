package dev.kamilklecha.autoswitchmobile;

import java.io.Serializable;
import java.sql.Time;
import java.util.Date;

public class Packet implements Serializable {
    public
    String Exec = "AddTask";
    String Action = "MonitorOff";
    String TimeMode = "Now";
    Integer seconds = 0;
    Time time = new Time(10,0,0);
    Date date = new Date(2019,05,22,22,00,00);
    Boolean force = false;
}
