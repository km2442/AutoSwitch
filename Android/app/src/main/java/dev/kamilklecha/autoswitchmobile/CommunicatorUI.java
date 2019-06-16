package dev.kamilklecha.autoswitchmobile;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;

import java.io.Serializable;

public class CommunicatorUI extends AppCompatActivity implements Serializable {

    String IP, Port;
    SocketCommunicator sc;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        IP = getIntent().getExtras().getString("IP");
        Port = getIntent().getExtras().getString("Port");
        SocketCommunicator sc = (SocketCommunicator) getIntent().getSerializableExtra("MyClass");;

        TextView tv = (TextView) findViewById(R.id.commUI_Addr);
        //tv.setText(IP + ':' + Port);

        setContentView(R.layout.activity_communicator_ui);
    }
}
