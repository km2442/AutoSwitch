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

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStreamReader;

public class PCSettings extends AppCompatActivity {

    private static final String TAG = "PCSettings";
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
    protected void onCreate(final Bundle savedInstanceState) {
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

        btnSave.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    saveSettings();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        });

        btnCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });

        try {
            getSettings();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        try {
            prepareSettingsUI();
        } catch (JSONException e) {
            e.printStackTrace();
        }

    }

    void getSettings() throws InterruptedException {
        Packet p = new Packet();
        p.Exec = "GetSettings";
        Hub.cui.sendMsg(p);

        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    BufferedReader in = new BufferedReader(new InputStreamReader(Hub.sc.socket.getInputStream()));
                    final String response = in.readLine();
                    setJson(response);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
        thread.start();
        thread.join();
    }

    void saveSettings() throws InterruptedException {
        Packet p = new Packet();
        p.Exec = "SetSettings";
        p.PCSettings = new SettingsPC();
        p.PCSettings.Language = toggleLanguage.getSelectedItemPosition();
        p.PCSettings.Theme = toggleTheme.getSelectedItemPosition();
        p.PCSettings.TrayVisible = toggleTray.isChecked();
        p.PCSettings.Statistics = toggleStatistics.isChecked();
        p.PCSettings.MultiInstance = toggleMultiInstance.isChecked();
        p.PCSettings.Logs = toggleLogging.isChecked();
        p.PCSettings.HideWarning = toggleWarningHide.isChecked();
        p.PCSettings.TestMode = toggleTestMode.isChecked();

        final Handler handler = new Handler(Looper.getMainLooper());

        Hub.cui.sendMsg(p);
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    BufferedReader in = new BufferedReader(new InputStreamReader(Hub.sc.socket.getInputStream()));
                    final String response = in.readLine();
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            if(response.equals("Settings Setted")) {
                                Toast.makeText(PCSettings.this, response, Toast.LENGTH_LONG).show();
                                finish();
                            }
                            else Toast.makeText(PCSettings.this, "Error while saving settings!", Toast.LENGTH_LONG).show();
                        }
                    });
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
        thread.start();
        thread.join();
    }

    void prepareSettingsUI() throws JSONException {
        JSONObject obj = new JSONObject(json);
        if(obj.has("Type") && obj.getString("Type").equals("Settings")) {
            toggleLanguage.setSelection(obj.getInt("Language"));
            toggleTheme.setSelection(obj.getInt("Theme"));
            toggleTray.setChecked(obj.getBoolean("TrayVisible"));
            toggleStatistics.setChecked(obj.getBoolean("Statistics"));
            toggleMultiInstance.setChecked(obj.getBoolean("MultiInstance"));
            toggleLogging.setChecked(obj.getBoolean("Logs"));
            toggleWarningHide.setChecked(obj.getBoolean("HideWarning"));
            toggleTestMode.setChecked(obj.getBoolean("TestMode"));

            loadingBar.setVisibility(View.INVISIBLE);
            settingsLayout.setVisibility(View.VISIBLE);
            btnSave.setEnabled(true);
        }
        else {
            Toast.makeText(PCSettings.this, "Can't acquire Settings from PC!", Toast.LENGTH_LONG).show();
            finish();
        }
    }
}
