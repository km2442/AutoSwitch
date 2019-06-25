package dev.kamilklecha.autoswitchmobile;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.support.constraint.ConstraintLayout;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.Spinner;
import android.widget.Toast;
import android.widget.ToggleButton;

import java.io.BufferedReader;
import java.io.InputStreamReader;

public class PCSettings extends AppCompatActivity {

    ProgressBar loadingBar;
    ConstraintLayout settingsLayout;

    Spinner toggleLanguage;
    Spinner toggleTheme;
    ToggleButton toggleTray;
    ToggleButton toggleStatistics;
    ToggleButton toggleMultiInstance;
    ToggleButton toggleLogging;
    ToggleButton toggleWarningHide;
    ToggleButton toggleTestMode;

    Button btnSave;
    Button btnCancel;

    public void setJson(String json) {
        this.json = json;
    }

    String json = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pcsettings);

        loadingBar = (ProgressBar) findViewById(R.id.PCSettings_LoadingBar);
        settingsLayout = (ConstraintLayout) findViewById(R.id.PCSettings_SettingsLayout);

        toggleLanguage = (Spinner) findViewById(R.id.PCSettings_LanguageSpinner);
        toggleTheme = (Spinner) findViewById(R.id.PCSettings_ThemeSpinner);
        toggleTray = (ToggleButton) findViewById(R.id.PCSettings_TrayToggle);
        toggleStatistics = (ToggleButton) findViewById(R.id.PCSettings_MultiInstanceToggle);
        toggleMultiInstance = (ToggleButton) findViewById(R.id.PCSettings_LoggingToggle);
        toggleLogging = (ToggleButton) findViewById(R.id.PCSettings_HideWarningToggle);
        toggleWarningHide = (ToggleButton) findViewById(R.id.PCSettings_HideWarningToggle);
        toggleTestMode = (ToggleButton) findViewById(R.id.PCSettings_TestModeToggle);

        btnSave = (Button) findViewById(R.id.PCSettings_SaveSettings);
        btnCancel = (Button) findViewById(R.id.PCSettings_Cancel);

        try {
            getSettings();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        loadingBar.setVisibility(View.INVISIBLE);
        settingsLayout.setVisibility(View.VISIBLE);
    }

    void getSettings() throws InterruptedException {
        Packet p = new Packet();
        p.Exec = "GetSettings";
        Hub.cui.sendMsg(p);
        final Handler handler = new Handler(Looper.getMainLooper());

        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    BufferedReader in = new BufferedReader(new InputStreamReader(Hub.sc.socket.getInputStream()));
                    final String response = in.readLine();

                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText(PCSettings.this, response, Toast.LENGTH_SHORT).show();
                        }
                    });
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
        thread.start();
//        Thread thread = new Thread(new Runnable() {
//            @Override
//            public void run() {
//                Socket s = Hub.sc.socket;
//                BufferedReader input = null;
//                try {
//                    input = new BufferedReader(new InputStreamReader(s.getInputStream()));
//                } catch (IOException e) {
    }
}
