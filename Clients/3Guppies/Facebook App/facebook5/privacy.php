<?php
	// The facebook client library
	include_once 'facebook-platform/php4client/facebook.php';

	// This defines some of your basic setup
	include_once 'config.php';
	include_once 'facebook.lib.php';
	//include_once 'dump.php';

	$pageName = 'Privacy';

	$facebook = new Facebook($api_key, $secret);
	$user = $facebook->get_loggedin_user();
	$userid = (int)Param('user', $user);
	$firstName = GetUserInfo($userid, 'first_name');

	$users = GetFriendsArray();
	$usersinfo = GetUsersInfoEx($users, 'first_name, last_name');
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
<head>
	<title></title>
	<?php include('fbstyles.part.htm'); ?>
	<style type="text/css"><?php include('styles.css'); ?></style>
	<script type="text/javascript"><?php include('fbscript.js'); ?></script>
</head>
<body class="fbframe">
	<?php include('header.part.php'); ?>

	<!-- ----------------------- MAIN CONTENT ----------------------- -->
	<div id="contentSpace">
		<div class="moduleWide" id="privacy_success">
			<div class="moduleBody white" style="padding: 10px;">
				<div class="textCenter">
					<table border="0" cellpadding="0" cellspacing="0" align="center">
						<tr>
							<td valign="middle">
								<a href="http://www.truste.org/ivalidate.php?url=www.mixxer.com&sealid=101" target="_blank">
									<img src="http://cdn.3guppies.com/images/clickseal.gif" border="0" style="margin-right: 8px;"></a>
							</td>
							<td valign="middle">
								<a href="http://www.bbbonline.org/cks.asp?id=207052110315788140" target="_blank">
									<img src="http://cdn.3guppies.com/images/PrivacySeal3.gif" border="0" style="margin-left: 8px;"></a>
							</td>
						</tr>
					</table>
				</div>
				<br />
				<span style="font-style: italic;">Last Updated: May 29, 2007</span><br />
				<br />
				<h2 class="textCenter">
					3Guppies, Inc. (&quot;3Guppies&quot;) takes your privacy seriously.<br />
					Please read this to learn about our policy regarding visitor and member privacy.</h2>
				<br />
				<br />
				3Guppies is a licensee of the TRUSTe Privacy Program. TRUSTe is an independent,
				non-profit organization whose mission is to build user�s trust and confidence in
				the Internet by promoting the use of fair information practices. This privacy statement
				covers the Web site 3guppies.com. Because this Web site wants to demonstrate its
				commitment to your privacy, it has agreed to disclose its information practices
				and have its privacy practices reviewed for compliance by TRUSTe.<br />
				<br />
				If you have questions or concerns regarding this statement, you should first contact
				3Guppies at <a href="mailto:customer-support@support.3guppies.com">customer-support@support.3guppies.com</a>.
				If you do not receive acknowledgement of your inquiry or your inquiry has not been
				satisfactorily addressed, you should contact TRUSTe at <a href="http://www.truste.org/consumers/watchdog_complaint.php"
					target="_blank">http://www.truste.org/consumers/watchdog_complaint.php</a>.
				TRUSTe will then serve as a liaison with us to resolve your concerns.<br />
				<br />
				3Guppies participates in the Council of Better Business Bureau's BBB<span style="font-style: italic;">OnLine</span>&reg;
				Privacy Program. As such, we have made a commitment to meet the program's strict
				requirements regarding how we treat your information and have it verified by BBB<span
					style="font-style: italic;">OnLine</span>. Further information about this program
				is available at <a href="http://www.bbbonline.org" target="_blank">http://www.bbbonline.org</a>.<br />
				<br />
				<strong>What this Privacy Policy Covers</strong><br />
				<br />
				This Privacy Policy covers 3Guppies' treatment of anonymous and personally identifiable
				information that 3Guppies collects when you are on the Web site located at www.3Guppies.com,
				and when you use 3Guppies' services. This policy also covers 3Guppies' treatment
				of any personally identifiable information that 3Guppies' business partners share
				with 3Guppies.<br />
				<br />
				This policy does not apply to the practices of companies that 3Guppies does not
				own or control or to people that 3Guppies does not employ or manage.<br />
				<br />
				<strong>Information Collection and Use</strong><br />
				<br />
				3Guppies logs non-personally-identifiable (anonymous) information including IP address,
				page requests, profile information, aggregate user data, and browser type, from
				visitors to the site. This data is used to manage the Web site, track usage and
				improve the Web site services. This non-personally-identifiable information may
				be shared with third-parties to provide more relevant services and advertisements
				to members.<br />
				<br />
				You can use 3Guppies� Web site by providing us your cell phone number and/or your
				cell phone carrier and model. Once you provide this information or register with
				3Guppies and sign in to our services, you are not anonymous to us. 3Guppies collects
				personally identifiable information when you register for a 3Guppies account and
				when you use the service. 3Guppies may also receive personally identifiable information
				from our business partners. When you register with 3Guppies, we may ask for your
				name, email address, and cell phone number, carrier, and model. When you purchase
				at 3Guppies, we may ask you for credit card and other billing information. You may
				also be requested to provide additional information to 3Guppies after you have registered,
				such as personal interests, gender, education and occupation in order to assist
				users in finding and communicating with each other.<br />
				<br />
				User Profile information including members' pictures and user names are displayed
				to people in order to facilitate user interaction in the 3Guppies community. Your
				full name and cell phone number will not be revealed to others unless you choose
				to make this information available in your profile. Email addresses may be used
				to send you notifications related to the service and your account; further, you
				may choose to use your email address to invite new friends to join 3Guppies and
				to receive promotional information.<br />
				<br />
				From time to time, 3Guppies or a partner may sponsor a promotion, sweepstake or
				contest on 3Guppies� Web site. You may be asked to provide personal information
				including name, email address or other information or to answer questions in order
				to participate. We may transfer personal information to certain ad partners if you
				have explicitly requested to receive information from them. It will be clear at
				the point of collection who is collecting the personal information and whose privacy
				policy will apply.<br />
				<br />
				In addition to processing your orders for the purchase of our services and products,
				we may use the information you provide to us for sending you special promotions
				and incentives, to customize the advertising and content you see on our pages, providing
				additional services, gathering demographic and website usage information, improving
				our website and the quality of your online visits, and in some cases, to contact
				you. With membership, your account is automatically set for you to receive special
				promotions and incentives, but you can opt out of this under My Settings.<br />
				<br />
				Please be aware that whenever you voluntarily post public information to the Web
				site, that information can be accessed by the public and can in turn be used by
				those people to send you unsolicited communications.<br />
				<br />
				Notwithstanding any other provision of this Privacy Policy, any information 3Guppies
				receives from you is an asset of 3Guppies and may be transferred if 3Guppies merges
				with another entity, is consolidated, sells all or part of its assets, or files
				for bankruptcy.<br />
				<br />
				<strong>Invitations and Other Communications to Non-members</strong><br />
				<br />
				On some pages, you can choose to send content to, or otherwise communicate with,
				a friend. In these instances, we may ask you to provide the recipient�s cell phone
				number, name, and/or email address. We will store this information for the sole
				purposes of delivering the content or message to your friend and to facilitate any
				further use of our services by your friend. If you choose to use our referral service
				to tell a friend about our site, we may ask you for your friend�s name, cell phone
				number, and email address. We will automatically send your friend a one-time email
				or text message inviting him or her to visit the site. We store this information
				for the sole purpose of sending this one-time message and tracking the success of
				our referral program. Your friend may contact us at customer-support@support.3Guppies.com
				to request that we remove this information from our database.<br />
				<br />
				<strong>Information Sharing and Disclosure</strong><br />
				<br />
				Except as otherwise described in this privacy statement, 3Guppies will not provide
				your personally identifiable information to other companies or people except in
				the following instances:
				<ul>
					<li>We have your consent to share the information;</li>
					<li>We need to share your information to provide the product or service you have requested;</li>
					<li>We need to send the information to companies who work on behalf of 3Guppies to provide
						a product or service to you;</li>
					<li>We respond to judicial proceedings, court orders or legal process; or</li>
					<li>We find that your actions on our Web sites violate the 3Guppies Terms of Use, or
						any of our usage guidelines for specific products or services.</li>
				</ul>
				<br />
				We reserve the right to disclose your personally identifiable information as required
				by law and when we believe that disclosure is necessary to protect our rights and/or
				comply with a judicial proceeding, court order, or legal process served on our Web
				site.<br />
				<br />
				We use a credit card processing company to bill you for goods and services. This
				company does not retain, share, store or use personally identifiable information
				for any other purposes. We use other third parties to provide marketing services,
				Web analytics, and Web hosting on our site. When you use our services, we will share
				your cell phone number, email address, and name as necessary for the third party
				to provide that service. These third parties are prohibited from using your personally
				identifiable information for any other purpose.<br />
				<br />
				We do not share, sell, rent or trade personally identifiable information with third
				parties for their promotional purposes.<br />
				<br />
				<strong>Cookies</strong><br />
				<br />
				Your Internet browser has a feature called "cookies" which may store small amounts
				of data on your computer about your visit to our site. 3Guppies and its agents may
				set and access 3Guppies cookies on your computer. When you interact with the site,
				we strive to make that experience easy and meaningful. When you come to the 3Guppies
				site, our web server uses cookies to improve your interaction with the site by,
				for example, remembering your preferences. We link the information we store in cookies
				to personally identifiable information you submit while on our site. As such, 3Guppies
				uses cookies to measure your behavior on the web site, to save personal settings
				on specific pages, to save your username and password so that 3Guppies can quickly
				log you in, to make sure you are who you say you are after you've logged in, and
				to identify the fact that you are a 3Guppies member or a prior visitor. If you disable
				their Web browsers' ability to accept cookies, you will still be able to browse
				the web site; however, you may lose some of the functionality provided by the use
				of cookies. Cookies tell us nothing about who you are unless you specifically give
				us personally identifiable information.<br />
				<br />
				3Guppies allows other companies that are presenting advertisements on some of our
				pages to set and access their cookies on your computer. Other companies' use of
				their cookies is subject to their own privacy policies, not this one. Advertisers
				or other companies do not have access to 3Guppies' cookies.<br />
				<br />
				3Guppies uses web beacons to access our cookies within our network of web sites
				and in connection with 3Guppies products and services.<br />
				<br />
				<strong>Your Ability to Edit and Delete Your Account Information and Preferences</strong><br />
				<br />
				You can change your profile at any time by logging into My 3Guppies with your user
				name and password. Once you have logged in, you can change your cellular phone model,
				cellular carrier, adult status, preferences, email address, telephone number and
				password. Any errors that cannot be corrected through My 3Guppies may be corrected
				by sending us a support request.<br />
				<br />
				We recognize that you may not want to receive offers or other information from 3Guppies.
				At any time, you may request to discontinue receiving offers or other information
				from us by replying to the email or regular mail you receive from us and informing
				us of your request. You may also call customer service at 1-877-464-9937 to opt-out.
				In any event, 3Guppies retains the right to communicate with you on non-promotional
				matters such as the status or use of your account. If you do not wish to receive
				any communications from 3Guppies, you can deactivate your account by contacting
				us at <a href="mailto:customer-support@support.3guppies.com">customer-support@support.3guppies.com</a>.<br />
				<br />
				<strong>Privacy of Children</strong><br />
				<br />
				3Guppies is not directed to children under the age of 13 and do not permit registration
				by, and will not knowingly collect or use personally identifiable information from,
				anyone under 13 years of age. We require users to be at least 14 years of age to
				use the web site.<br />
				<br />
				<strong>Security</strong><br />
				<br />
				The security of your personal information is important to us. Your 3Guppies account
				information is password-protected for your privacy and security. Further, when you
				enter sensitive information (such as credit card number) on our registration or
				order forms, we encrypt that information using secure socket layer technology (SSL).<br />
				<br />
				We follow generally accepted industry standards to protect the personal information
				submitted to us, both during transmission and once we receive it. No method of transmission
				over the Internet, or method of electronic storage, is 100% secure, however. Therefore,
				while we strive to use commercially acceptable means to protect your personal information,
				we cannot guarantee its absolute security.<br />
				<br />
				If you have any questions about security on our Web site, you can send email us
				at <a href="mailto:customer-support@support.3guppies.com">customer-support@support.3guppies.com</a>.<br />
				<br />
				<strong>Changes to this Privacy Policy</strong><br />
				<br />
				We reserve the right to modify this privacy policy at any time, so please review
				it frequently. If we make material changes to this policy, we will notify you here,
				by email, or by means of a notice on our home page.<br />
				<br />
				<strong>Questions or Suggestions</strong><br />
				<br />
				If you have questions or suggestions, please send an email to <a href="mailto:termsofuse@3guppies.com">
					termsofuse@3guppies.com</a> or mail us at 3Guppies, Inc., 100 S. King St, #320,
				Seattle, WA 98104.<br />
				<br />
			</div>
		</div>
	</div>
	<!-- /END Main Content -->

	<div style="position: relative; bottom: 0;">
		<?php include('footer.part.php'); ?>
	</div>
</body>
</html>
