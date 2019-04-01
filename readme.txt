    这是我的本科毕业设计《空间目标识别与跟踪算法》
    任务是在视频流中点击目标，自动给出目标bbox并对其在接下来的帧中继续跟踪
    三月开始，三月二十七日完成，初次完成时功能简单，想着导师指点一二再精近修改，奈何导师觉得oK自己也就懒得修改了

    本次实现 是使用YOLOV3检测 + ECO-HC跟踪
    因为我们主要是应用在空天环境，待检测目标往往较小也比较稀疏，考虑到two-stage检测过慢，one-stage的SSD，YOLO，YOLOV2又对小目标不敏感，故选择了YOLOV3（虽然本人不喜DL，But实验室学长们也没用DL，导师希望我往这方面扩展一下经验）；
    目前的tracker方=也已经被DL占领了，现在的tracker到底是传统的工程问题还是online learning已经不重要了，因为衡量的标准是public  dataset的精度。非DLtracker最厉害的唯CF系列莫属了（这导师你就管不了了吧^-^,这也是本人下一步的主要学习内容了），针对前几代CF的问题，MD大神的最新力作ECO一出世便惊艳了世人。鉴于本人小笔记本的性能有限，故选择了青春版但是性能同样在线飞起的ECO-HC。


chengxinnn
Salute to the goddess;Fight for the goddess.
