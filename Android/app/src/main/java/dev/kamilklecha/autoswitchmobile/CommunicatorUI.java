package dev.kamilklecha.autoswitchmobile;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.google.gson.Gson;

import java.io.IOException;
import java.io.Serializable;

public class CommunicatorUI extends AppCompatActivity implements Serializable {

    String addr;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        addr = getIntent().getExtras().getString("Addr");

        setContentView(R.layout.activity_communicator_ui);

        TextView tv = (TextView) findViewById(R.id.commUI_Addr);
        tv.setText(addr);

        Button btnShutdown = (Button) findViewById(R.id.commUI_Shutdown);
        Button btnReboot = (Button) findViewById(R.id.commUI_Reboot);
        Button btnSleep = (Button) findViewById(R.id.commUI_Sleep);
        Button btnHibernate = (Button) findViewById(R.id.commUI_Hibernate);
        Button btnLogout = (Button) findViewById(R.id.commUI_Logout);
        Button btnLock = (Button) findViewById(R.id.commUI_Lock);
        Button btnMonitorOff = (Button) findViewById(R.id.commUI_MonitorOff);
        Button btnSettings = (Button) findViewById(R.id.commUI_Settings);
        Button btnDisconnect = (Button) findViewById(R.id.commUI_Disconnect);

        btnShutdown.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ShutdownPC();
            }
        });

        btnReboot.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                RebootPC();
            }
        });

        btnSleep.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SleepPC();
            }
        });

        btnHibernate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                HibernatePC();
            }
        });

        btnLogout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                LogoutUser();
            }
        });

        btnLock.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                LockPC();
            }
        });

        btnMonitorOff.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                TurnMonitorOff();
            }
        });

        btnSettings.setEnabled(false);

        btnDisconnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Disconnect();
            }
        });
    }

    private void ShutdownPC() {
        Packet p = new Packet();
        p.Exec = "AddTask";
        p.Action = "Shutdown";
        p.TimeMode = "Now";
        sendMsg(p);
    }

    private void RebootPC() {
        Packet p = new Packet();
        p.Exec = "AddTask";
        p.Action = "Reboot";
        p.TimeMode = "Now";
        sendMsg(p);
    }

    private void SleepPC() {
        Packet p = new Packet();
        p.Exec = "AddTask";
        p.Action = "Sleep";
        p.TimeMode = "Now";
        sendMsg(p);
    }

    private void HibernatePC() {
        Packet p = new Packet();
        p.Exec = "AddTask";
        p.Action = "Hibernate";
        p.TimeMode = "Now";
        sendMsg(p);
    }

    private void LogoutUser() {
        Packet p = new Packet();
        p.Exec = "AddTask";
        p.Action = "Logout";
        p.TimeMode = "Now";
        sendMsg(p);
    }

    private void LockPC() {
        Packet p = new Packet();
        p.Exec = "AddTask";
        p.Action = "Block";
        p.TimeMode = "Now";
        sendMsg(p);
    }

    private void TurnMonitorOff() {
        Packet p = new Packet();
        p.Exec = "AddTask";
        p.Action = "MonitorOff";
        p.TimeMode = "Now";
        sendMsg(p);
    }

    private void OpenSettings() {

    }

    private void sendMsg(Packet p) {
        Gson gson = new Gson();
        String json = gson.toJson(p);
        Hub.sc.sendMessage(json);
    }

    private void Disconnect() {
        try {
            Hub.sc.socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        finish();
    }
}
