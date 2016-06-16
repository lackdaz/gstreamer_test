---
short-description: Deploy the SDK with your application
...

# Deploying your application

Once the development of your application is finished, you will need to
deploy it to the target machine, usually in the form of a package or
installer. You have several options here, and, even though this subject
is not really in the scope of this documentation, we will give some
hints to try to help you.

# Multiplatform vs. single-platform packaging system

The first choice you need to make is whether you want to deploy your
application to more than one platform. If yes, then you have the choice
to use a different packaging system for each platform, or use one that
can deliver to all platforms simultaneously. This table summarizes the
pros and cons of each option.

<table>
<colgroup>
<col width="33%" />
<col width="33%" />
<col width="33%" />
</colgroup>
<thead>
<tr class="header">
<th> </th>
<th>Pros</th>
<th>Cons</th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td><p><strong><span style="color: rgb(0,0,0);">Multiplatform packaging </span></strong><span style="color: rgb(0,0,0);"><strong>system</strong></span></p>
<p><span style="color: rgb(0,0,0);"> </span>The same system is used to package your application for all platforms</p></td>
<td><ul>
<li><p>You only need to develop your packaging system once, and it works for all supported platforms.</p></li>
</ul></td>
<td><ul>
<li>On some platforms, the packaging system might impose artificial restrictions inherited from the other platforms.</li>
</ul></td>
</tr>
<tr class="even">
<td><p><strong>Single-platform packaging system</strong></p>
<p>Your application is packaged using a different system on each platform.</p></td>
<td><ul>
<li><p>You can make use of all the advantages each <span>packaging </span>system can offer.</p>
<p> </p></li>
</ul></td>
<td><ul>
<li><p>You need to develop a new <span>packaging </span>system for each supported platform.</p></li>
</ul></td>
</tr>
</tbody>
</table>

The GStreamer SDK itself supports three different platforms (Linux, Mac
OS X and Windows) and has been built using a multiplatform packaging
system named **Cerbero**, which is available for you to use, should you
choose to go down this route.

# Shared vs. private GStreamer deployment

You can install the GStreamer SDK in the target machine in the same way
you installed it in your development machine, you can deploy it
privately, or you can even customize it before deploying. Here you have
a few options:

<table>
<colgroup>
<col width="33%" />
<col width="33%" />
<col width="33%" />
</colgroup>
<thead>
<tr class="header">
<th> </th>
<th>Pros</th>
<th>Cons</th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td><p><strong>Shared SDK</strong></p>
<p>The GStreamer SDK is installed independently of your application, as a prerequisite, in a common place in the target computer (<code>C:\Program Files</code>, for example). You application uses an environment variable to locate it.</p></td>
<td><ul>
<li><p>If more than one application in the target computer uses the SDK, it is installed only once and shared, reducing disk usage.</p></li>
</ul></td>
<td><ul>
<li>Tampering or corruption of the shared SDK installation can make your application fail.</li>
<li><p>The SDK libraries are unprotected and open to tampering.</p></li>
</ul></td>
</tr>
<tr class="even">
<td><p><strong>Private SDK with dynamic linking</strong></p>
<p>Your application deploys the GStreamer SDK to a private folder.</p></td>
<td><ul>
<li><p>Your SDK is independent of other applications, so it does not get corrupted if other applications mess with their installations.</p></li>
</ul></td>
<td><ul>
<li><p>If multiple applications in the target computer use the GStreamer SDK, it won’t be shared, consuming more disk space.</p></li>
<li><p>The SDK libraries are unprotected and open to tampering.</p></li>
</ul></td>
</tr>
<tr class="odd">
<td><p><strong>Private SDK with static linking</strong></p>
<p>Your application links statically against the GStreamer SDK, so it effectively becomes part of your application binary.</p></td>
<td><ul>
<li>Your SDK is independent of other applications, so it does not get corrupted if other applications mess with their installations.</li>
<li>It is much harder to tamper with the SDK, since it is embedded in your application.</li>
</ul></td>
<td><ul>
<li><span>If multiple applications in the target computer use the GStreamer SDK, it won’t be shared, consuming more disk space.</span></li>
</ul></td>
</tr>
</tbody>
</table>

The following pages give further directions for some of the above
options.

  - Platform-specific packaging methods:
      - For [Mac OS X](Mac%2BOS%2BX%2Bdeployment.html)
      - For [Windows](Windows%2Bdeployment.html)
  - [Multiplatform deployment using
    Cerbero](Multiplatform%2Bdeployment%2Busing%2BCerbero.html)

 