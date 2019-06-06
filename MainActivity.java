package com.example.smartbin;

import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.nfc.NdefMessage;
import android.nfc.NfcAdapter;
import android.nfc.tech.NfcA;
import android.os.Parcelable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.widget.TextView;
import android.widget.Toast;


public class MainActivity extends AppCompatActivity {

    NfcAdapter mAdapter;
    int credits = 0;
    TextView tvcr;
    public static final String EXTRA_MESSAGE = "com.example.smartbin.extra.credits";
    SharedPreferences sharedPref;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        sharedPref = getSharedPreferences("mypref", 0);
        int credits = sharedPref.getInt("credit", 0);

        mAdapter = NfcAdapter.getDefaultAdapter(this);

        Intent intent = getIntent();
        String action = intent.getAction();
        tvcr = findViewById(R.id.creditId);
        tvcr.setText((String.valueOf(credits)));

        if (NfcAdapter.ACTION_NDEF_DISCOVERED.equals(action)){
            parseNdefMessage(intent);
            tvcr.setText((String.valueOf(credits)));

        }
    }
    @Override
    public void onNewIntent (Intent intent){
        String action = intent.getAction();
        if(NfcAdapter.ACTION_NDEF_DISCOVERED.equals(action)){
            parseNdefMessage(intent);
            tvcr.setText((String.valueOf(credits)));
            finish();
        }
    }

    private static long back_pressed;
    @Override
    public void onBackPressed(){
        if (back_pressed + 2000 > System.currentTimeMillis()){
            finish();
            //super.onBackPressed();
        }
        else{
            Toast.makeText(getBaseContext(), "Press once again to exit", Toast.LENGTH_SHORT).show();
            back_pressed = System.currentTimeMillis();
        }
    }

    private void process(Intent intent){
        Parcelable[] parcelabled = intent.getParcelableArrayExtra(NfcAdapter.EXTRA_NDEF_MESSAGES);
        if(parcelabled != null){
            Toast.makeText(getApplicationContext(), "NDEF TAG DETECTED", Toast.LENGTH_LONG).show();
            /*NdefMessage[] ndefmsg = new NdefMessage[parcelabled.length];
            for(int i = 0; i < parcelabled.length; i++){
                ndefmsg[i] = (NdefMessage) parcelabled[i];
            }
            for(int i = 0; i < ndefmsg[0].getRecords().length; i++){
                Log.d("NDEF Intent Process", ndefmsg[0].getRecords()[i].toString());
            }*/

        }
        return;
    }

    void parseNdefMessage(Intent intent) {
        Parcelable[] ndefMessageArray = intent.getParcelableArrayExtra(NfcAdapter.EXTRA_NDEF_MESSAGES);
        NdefMessage ndefMessage = (NdefMessage) ndefMessageArray[0];
        String msg = new String(ndefMessage.getRecords()[0].getPayload());
        String nuova = msg.substring(3);
        //Toast.makeText(getApplicationContext(), nuova, Toast.LENGTH_LONG).show();
        if(nuova.equals("YES")){
            Intent myIntent = new Intent(MainActivity.this, RetrieveCrActivity.class);
            myIntent.putExtra(EXTRA_MESSAGE, credits);
            MainActivity.this.startActivity(myIntent);
        } else {
            Toast.makeText(getApplicationContext(), "This is not metal!", Toast.LENGTH_LONG).show();
        }
    }


    @Override
    public void onResume(){
        super.onResume();
        PendingIntent intent = PendingIntent.getActivity(this, 0, new Intent(this, getClass()).addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP), 0);
        NfcAdapter.getDefaultAdapter(this).enableForegroundDispatch(this, intent, null, null);
        Bundle extras = getIntent().getExtras();
        if (extras != null) {
            credits = extras.getInt("credits");
        }
        tvcr = findViewById(R.id.creditId);
        tvcr.setText((String.valueOf(credits)));
    }

    @Override
    protected void onPause(){
        super.onPause();
        if(NfcAdapter.getDefaultAdapter(this) != null)
            NfcAdapter.getDefaultAdapter(this).disableForegroundDispatch(this);
        tvcr = findViewById(R.id.creditId);
        tvcr.setText((String.valueOf(credits)));
    }

}
