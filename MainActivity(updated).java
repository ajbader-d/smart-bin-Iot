package com.example.smartbin;

import android.app.PendingIntent;
import android.content.Intent;
import android.nfc.NdefMessage;
import android.nfc.NfcAdapter;
import android.os.Parcelable;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.firebase.ui.auth.AuthUI;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.HashMap;

public class MainActivity extends AppCompatActivity {

    NfcAdapter mAdapter;
    Integer credits;
    TextView tvcr;
    public static final String EXTRA_MESSAGE = "com.example.smartbin.extra.credits";

    FirebaseAuth mAuth = FirebaseAuth.getInstance();
    FirebaseAuth firebaseAuth;
    FirebaseAuth.AuthStateListener mAuthListener;
    FirebaseDatabase mFirDb;
    DatabaseReference myRef;
    DatabaseReference current_user_db;

    Button next, sign_out_btn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home);
        sign_out_btn = (Button)findViewById(R.id.sign_out_btn);
        sign_out_btn.setEnabled(true);

        firebaseAuth = FirebaseAuth.getInstance();
        mFirDb = FirebaseDatabase.getInstance();
        myRef = mFirDb.getReference();
        FirebaseUser user = FirebaseAuth.getInstance().getCurrentUser();
        String user_id = user.getUid();

        myRef.child("Users").child(user_id).child("credits").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                Long temp = (Long) dataSnapshot.getValue();
                credits = temp.intValue();
                tvcr = findViewById(R.id.creditId);
                tvcr.setText((String.valueOf(credits)));
            }
            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });

        mAdapter = NfcAdapter.getDefaultAdapter(this);

        next = (Button)findViewById(R.id.temp_btn);
        next.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent myIntent = new Intent(MainActivity.this, RetrieveCrActivity.class);
                myIntent.putExtra(EXTRA_MESSAGE, credits);
                MainActivity.this.startActivity(myIntent);
                finish();
            }
        });

        sign_out_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AuthUI.getInstance()
                        .signOut(MainActivity.this)
                        .addOnCompleteListener(new OnCompleteListener<Void>() {
                            @Override
                            public void onComplete(@NonNull Task<Void> task) {
                                sign_out_btn.setEnabled(false);
                                Intent myIntent = new Intent(MainActivity.this, LogInActivity.class);
                                MainActivity.this.startActivity(myIntent);

                            }
                        }).addOnFailureListener(new OnFailureListener() {
                    @Override
                    public void onFailure(@NonNull Exception e) {
                        Toast.makeText(MainActivity.this, ""+e.getMessage(),Toast.LENGTH_SHORT).show();
                    }
                });

            }
        });

        Intent intent = getIntent();
        String action = intent.getAction();
        tvcr = findViewById(R.id.creditId);
        tvcr.setText((String.valueOf(credits)));

        if (NfcAdapter.ACTION_NDEF_DISCOVERED.equals(action)){
            parseNdefMessage(intent);
            tvcr.setText((String.valueOf(credits)));
        }
        TextView user_tv = (TextView) findViewById(R.id.nomeId);
        user_tv.setText(""+mAuth.getCurrentUser().getDisplayName());
    }

    @Override
    public void onNewIntent (Intent intent){
        String action = intent.getAction();
        if(NfcAdapter.ACTION_NDEF_DISCOVERED.equals(action)){
            parseNdefMessage(intent);
            tvcr.setText((String.valueOf(credits)));
        }
    }

    private static long back_pressed;
    @Override
    public void onBackPressed(){
        if (back_pressed + 2000 > System.currentTimeMillis()){
            finish();
        }
        else{
            Toast.makeText(getBaseContext(), "Press once again to exit", Toast.LENGTH_SHORT).show();
            back_pressed = System.currentTimeMillis();
        }
    }
    void parseNdefMessage(Intent intent) {
        Parcelable[] ndefMessageArray = intent.getParcelableArrayExtra(NfcAdapter.EXTRA_NDEF_MESSAGES);
        NdefMessage ndefMessage = (NdefMessage) ndefMessageArray[0];
        String msg = new String(ndefMessage.getRecords()[0].getPayload());
        String nuova = msg.substring(3);
        if(nuova.equals("YES")){
            Intent myIntent = new Intent(MainActivity.this, RetrieveCrActivity.class);
            myIntent.putExtra(EXTRA_MESSAGE, credits);
            MainActivity.this.startActivity(myIntent);
        } else {
            Toast.makeText(getApplicationContext(), "This is not metal!", Toast.LENGTH_LONG).show();
        }
    }
    public void onResume(){
        super.onResume();
        PendingIntent intent = PendingIntent.getActivity(this, 0, new Intent(this, getClass()).addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP), 0);
        NfcAdapter.getDefaultAdapter(this).enableForegroundDispatch(this, intent, null, null);
        Bundle extras = getIntent().getExtras();
        if (extras != null) {
            credits = extras.getInt("credits");
            FirebaseUser user = FirebaseAuth.getInstance().getCurrentUser();
            String user_id = user.getUid();
            DatabaseReference current_user_db = FirebaseDatabase.getInstance().getReference().child("Users").child(user_id);
            HashMap map = new HashMap();
            map.put("credits", credits);
            current_user_db.updateChildren(map);
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
