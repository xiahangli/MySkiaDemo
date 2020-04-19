package com.example.myskiademo;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import androidx.core.app.ActivityCompat;

import java.io.DataOutputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        int REQUEST_EXTERNAL_STORAGE = 1;
        String[] perm = {
                Manifest.permission.READ_EXTERNAL_STORAGE,
                Manifest.permission.WRITE_EXTERNAL_STORAGE
        };
        int permission = ActivityCompat.checkSelfPermission(MainActivity.this, Manifest.permission.WRITE_EXTERNAL_STORAGE);

        if (permission != PackageManager.PERMISSION_GRANTED) {
            // We don't have permission so prompt the user
            ActivityCompat.requestPermissions(
                    MainActivity.this,
                    perm,
                    REQUEST_EXTERNAL_STORAGE
            );
        }
        findViewById(R.id.sample_text).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread() {
                    @Override
                    public void run() {
                        super.run();
                        FileInputStream fileInputStream = null;
                        try {
                            fileInputStream = new
                                    FileInputStream("/dev/graphics/fb0");
                            if (fileInputStream == null)
                                Log.i("RUN", "run null");
                            else
                                Log.i("RUN", "not null");
                        } catch (FileNotFoundException e) {
                            e.printStackTrace();
                        } finally {
                            if (fileInputStream != null)
                                try {
                                    fileInputStream.close();
                                } catch (IOException e) {
                                    e.printStackTrace();
                                }
                        }
                    }
                }.start();
            }
        });
        execLinuxCmd("chmod 777 /dev/graphics/fb0");
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == 1){
            System.out.println();
        }
    }

    public void execLinuxCmd(String cmd) {
        Process process = null;
        DataOutputStream os = null;
        try {
            process = Runtime.getRuntime().exec("su");
            os = new DataOutputStream(process.getOutputStream());
            os.writeBytes(cmd + "\n");
            os.writeBytes("exit\n");
            os.flush();
            process.waitFor();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (os != null) {
                try {
                    os.close();
                    process.destroy();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public void testButtonClicked(View view) {
        new Thread() {
            @Override
            public void run() {
                super.run();
                FileInputStream fileInputStream = null;
                try {
                    fileInputStream = new 
                    FileInputStream("/dev/graphics/fb0");      
                    if (fileInputStream == null)
                        Log.i("RUN", "run null");
                    else
                        Log.i("RUN", "not null");
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                } finally {
                    if (fileInputStream != null)
                        try {
                            fileInputStream.close();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                }
            }
        }.start();
    }
}
